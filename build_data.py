#!/usr/bin/env python3
"""Parse README.md into data.js for the cp2go tracker.

The README stays the single source of truth. Re-run this after any upstream
pull to refresh the app's content:

    python3 build_data.py

Output: data.js  (a `window.CP2GO = {...}` assignment, no build tooling,
no dependencies -- the app is a static page opened straight from disk.)
"""
import hashlib
import json
import re
import sys
from pathlib import Path
from urllib.parse import urlparse

HERE = Path(__file__).resolve().parent
README = HERE / "README.md"
OUT = HERE / "data.js"

# --------------------------------------------------------------------------
# platform detection: gives the app a filter axis and a favicon-ish badge
# --------------------------------------------------------------------------
PLATFORMS = [
    ("codeforces", "Codeforces"),
    ("codechef", "CodeChef"),
    ("hackerrank", "HackerRank"),
    ("hackerearth", "HackerEarth"),
    ("leetcode", "LeetCode"),
    ("spoj", "SPOJ"),
    ("atcoder", "AtCoder"),
    ("vjudge", "vJudge"),
    ("onlinejudge", "UVa"),
    ("youtube", "YouTube"),
    ("cp-algorithms", "cp-algorithms"),
    ("topcoder", "TopCoder"),
    ("anudeep2011", "Blog"),
    ("adilet", "Blog"),
    ("math.spbu", "PDF"),
]
VIDEO_EMOJI = ":movie_camera:"


def platform_of(url):
    host = urlparse(url).netloc.lower()
    for frag, label in PLATFORMS:
        if frag in host:
            return label
    return host.replace("www.", "") or "Link"


def item_id(month, week, title, url):
    """Stable across upstream edits that only reorder the list.

    Hashes the identity of the row (month + week + title + url) rather than its
    position, so progress survives someone inserting a problem above yours.
    """
    raw = f"{month}|{week}|{title}|{url}".encode()
    return hashlib.blake2s(raw, digest_size=5).hexdigest()


# --------------------------------------------------------------------------
# line-level scrapers
# --------------------------------------------------------------------------
RE_HEADING = re.compile(r"^\s*(?:\d+\.\s*)?(#{2,6})\s*(.+?)\s*$")
RE_WEEK = re.compile(r"<summary>\s*(Week\s+(\d+))\s*</summary>", re.I)
RE_MONTH = re.compile(r"^#\s+Month\s+(\d+)\s*$", re.I)
RE_CHECK = re.compile(r"^(\s*)[-*]\s+\[( |x|X)\]\s+(.*)$")
RE_GROUP = re.compile(r"^\s*[-*]\s+(?!\[)(.+?)\s*:?\s*$")
RE_TABLE = re.compile(r"^\|(.*)\|\s*$")

# [- [ ] :movie_camera:](yt)[Title](page)  ->  video flag + two links
RE_ITEM_LINKS = re.compile(r"\[([^\]]*)\]\(([^)]+)\)")
RE_TRAILING_NOTE = re.compile(r"\s*\(([^()]*(?:finding|using|read|and)\s.*)\)\s*$", re.I)


def stars_to_diff(cell):
    """'★★☆' -> 2. A bare ☆ or a missing cell counts as 1."""
    full, half = cell.count("★"), cell.count("⯨") + cell.count("⯪")
    n = full + (0.5 if half else 0)
    if n == 0:
        return 1 if "☆" in cell else 0
    return int(round(n))


def parse_table_row(line, ctx):
    """`| ★★☆ | [Title](url) | <ul> <li> [ ] </li> </ul> |` -> one item."""
    cells = [c.strip() for c in line.strip().strip("|").split("|")]
    if len(cells) < 2:
        return None
    diff = stars_to_diff(cells[0])
    links = RE_ITEM_LINKS.findall(cells[1])
    if not links:
        return None
    title, url = links[0]
    # trailing parenthetical in the title is a note, not part of the name
    m = RE_TRAILING_NOTE.search(title)
    note = None
    if m:
        note, title = m.group(1).strip(), title[: m.start()].strip()
    return make_item(title, url, ctx, diff=diff, note=note)


def make_item(title, url, ctx, diff=0, note=None, video_url=None):
    month, week, section, kind = ctx
    title = title.replace(VIDEO_EMOJI, "").strip()
    if not title:
        return None
    video = bool(video_url) or "youtube.com" in url or "youtu.be" in url
    note = note or None
    return {
        "id": item_id(month, week, title, url),
        "t": title,
        "u": url,
        "k": kind,
        "d": diff,
        "p": platform_of(url),
        "v": bool(video),
        **({"vu": video_url} if video_url else {}),
        **({"n": note} if note else {}),
        "m": month,
        "w": week,
        "s": section,
    }


def kind_from_section(heading):
    """Map a markdown heading onto the three kinds the app filters on."""
    h = heading.lower()
    if "contest" in h:
        return "contest"
    if "reading" in h or "tutorial" in h or "note" in h or "material" in h:
        return "read"
    return "problem"


def ensure_section(cur_week, section, kind="problem"):
    """Some weeks list items before any heading (Week 1 does), so the section
    list can legitimately be empty. Open one on demand.

    Returns (section, sections_dict) -- the caller must rebind `section`,
    since a new section becomes the active one."""
    if not cur_week["sections"]:
        # Week 1 opens with three tutorials before any heading. They are the
        # week's reading list, so name them that instead of "General".
        label = section[0] if section else "Reading Material"
        k = section[1] if section else "read"
        cur_week["sections"].append({"t": label, "k": k, "items": []})
        section = (label, k)
    return section, cur_week["sections"][-1]


# --------------------------------------------------------------------------
# main parse
# --------------------------------------------------------------------------
def parse(text):
    months = []
    cur_month = None
    cur_week = None          # dict being built
    section = None           # (title, kind)
    week_topic = None
    topic_name = ""          # most recent level-3 heading, for disambiguation

    lines = text.splitlines()
    i = 0
    while i < len(lines):
        raw = lines[i]
        line = raw.rstrip()
        stripped = line.strip()

        m = RE_MONTH.match(stripped)
        if m:
            cur_month = {"n": int(m.group(1)), "weeks": []}
            months.append(cur_month)
            cur_week = None
            i += 1
            continue

        # <details><summary>Week N</summary>
        m = RE_WEEK.search(line)
        if m:
            cur_week = {
                "n": int(m.group(2)),
                "title": None,
                "sections": [],
            }
            if cur_month is None:  # defensive: content before any "# Month"
                cur_month = {"n": 1, "weeks": []}
                months.insert(0, cur_month)
            cur_month["weeks"].append(cur_week)
            section = None
            week_topic = None
            topic_name = ""
            i += 1
            continue

        if "</details>" in stripped:
            cur_week, section, week_topic, topic_name = None, None, None, ""
            i += 1
            continue

        if cur_week is None:
            i += 1
            continue

        # ---- section heading -------------------------------------------------
        # Two forms: `#### Problems` and the numbered `1. #### Reading material`.
        # Numbered ones are matched first -- a `###` topic (Sparse Table) makes
        # its numbered children SUB-blocks, so the app can show
        # "Sparse Table > Reading Material" rather than two loose sections.
        m = RE_HEADING.match(line)
        if m:
            level = len(m.group(1))
            heading = m.group(2).replace("`", "").strip()
            heading = re.sub(r"^\d+\.\s*", "", heading)
            k = kind_from_section(heading)
            # Sections stay flat. A week legitimately repeats a heading under
            # different topics (Week 5: "Problems" under Sparse Table and again
            # under Disjoint Set Union), so duplicates get the nearest preceding
            # level-3 topic prefixed rather than being nested -- nesting by
            # heading depth silently swallowed whole sections when an upstream
            # edit shifted a heading's level.
            label = heading
            if level <= 3 and heading.lower() != topic_name:
                topic_name = heading
            dup = any(s["t"] == label for s in cur_week["sections"])
            if dup and topic_name and topic_name.lower() != heading.lower():
                label = f"{topic_name} · {heading}"
            cur_week["sections"].append(
                {"t": label, "k": k, "items": []}
            )
            section = (heading, k)
            # "Week 4: Practice Contest" names both the week and its only
            # section; strip the prefix from both so neither shows it twice.
            # Only touch the label when there was a prefix -- otherwise this
            # would overwrite the duplicate-title disambiguation above.
            if cur_week["title"] is None:
                cur_week["title"] = re.sub(
                    r"^Week\s*\d+\s*[:\-]\s*", "", heading, flags=re.I
                )
            stripped_heading = re.sub(
                r"^Week\s*\d+\s*[:\-]\s*", "", heading, flags=re.I
            )
            if stripped_heading != heading:
                cur_week["sections"][-1]["t"] = stripped_heading
            i += 1
            continue

        # the bare line right after <summary> is the week's topic
        if (
            stripped
            and week_topic is None
            and not stripped.startswith(("#", "-", "|", "*", "<"))
            and stripped != "***"
        ):
            cur_week["title"] = stripped.rstrip(":").strip()
            i += 1
            continue


        # ---- table row ------------------------------------------------------
        if stripped.startswith("|"):
            if set(stripped) <= set("|- "):      # separator row
                i += 1
                continue
            ctx = (cur_month["n"], cur_week["n"], section[0], section[1])
            it = parse_table_row(stripped, ctx)
            if it:
                section, sect = ensure_section(cur_week, section)
                sect["items"].append(it)
            i += 1
            continue

        # ---- checkbox item --------------------------------------------------
        m = RE_CHECK.match(line)
        if m:
            indent, _, body = m.groups()
            links = RE_ITEM_LINKS.findall(body)
            if links:
                # A `:movie_camera:` row is a pair:
                #   [:movie_camera:](yt)[Name](real-page)
                # The emoji lives INSIDE the first link's text. The real
                # resource is the second link; the yt one is a bonus video.
                video_url = None
                if VIDEO_EMOJI in links[0][0]:
                    video_url = links[0][1]
                    links = links[1:]
                    if not links:
                        i += 1
                        continue
                title, url = links[0]
                ctx = (cur_month["n"], cur_week["n"], section[0] if section else "General", section[1] if section else "problem")
                it = make_item(title, url, ctx, video_url=video_url)
                if it:
                    section, sect = ensure_section(cur_week, section)
                    sect["items"].append(it)
            i += 1
            continue

        # ---- group header, e.g. "- Solve the following MISC problems:" -----
        if stripped.startswith(("- ", "* ")):
            m = RE_GROUP.match(line)
            if m and "[" not in m.group(1):
                label = m.group(1).strip().rstrip(":")
                k = kind_from_section(label)
                cur_week["sections"].append({"t": label, "k": k, "items": []})
                section = (label, k)
        i += 1

    return months


def tidy(months):
    """Drop empty sections/weeks, then verify nothing was lost."""
    for mo in months:
        kept = []
        for wk in mo["weeks"]:
            wk["sections"] = [s for s in wk["sections"] if s["items"]]
            if wk["sections"]:
                kept.append(wk)
        mo["weeks"] = kept
    return [m for m in months if m["weeks"]]


def count_checkboxes(text):
    n = 0
    for line in text.splitlines():
        if RE_CHECK.match(line):
            n += 1
        elif line.strip().startswith("|"):
            cells = [c.strip() for c in line.strip().strip("|").split("|")]
            if len(cells) >= 3 and RE_ITEM_LINKS.search(cells[1]) and "[ ]" in cells[-1]:
                n += 1
    return n


def main():
    text = README.read_text(encoding="utf-8")
    months = tidy(parse(text))

    items = [it for mo in months for wk in mo["weeks"]
             for s in wk["sections"] for it in s["items"]]
    expected = count_checkboxes(text)
    got = len(items)
    if got != expected:
        print(f"WARNING: parsed {got} items but README has {expected} checkboxes", file=sys.stderr)

    seen, dupes = set(), []
    for it in items:
        if it["id"] in seen:
            dupes.append(it["id"])
        seen.add(it["id"])

    payload = {"months": months, "stats": {
        "problems": sum(1 for it in items if it["k"] == "problem"),
        "reading": sum(1 for it in items if it["k"] == "read"),
        "contests": sum(1 for it in items if it["k"] == "contest"),
        "total": len(items),
        "platforms": len({it["p"] for it in items}),
    }}

    OUT.write_text(
        "// GENERATED by build_data.py from README.md -- do not edit by hand.\n"
        "// Re-run `python3 build_data.py` after pulling upstream changes.\n"
        "window.CP2GO = " + json.dumps(payload, ensure_ascii=False, indent=1) + ";\n",
        encoding="utf-8",
    )

    print(f"wrote {OUT.name}: {len(months)} months, "
          f"{sum(len(m['weeks']) for m in months)} weeks, {got} items "
          f"({expected} checkboxes in README)")
    print(f"  problems={payload['stats']['problems']} reading={payload['stats']['reading']} "
          f"contests={payload['stats']['contests']}")
    if dupes:
        print(f"  duplicate ids: {dupes}", file=sys.stderr)


if __name__ == "__main__":
    main()