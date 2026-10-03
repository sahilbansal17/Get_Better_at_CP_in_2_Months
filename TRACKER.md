# cp2go

An interactive tracker for the **Get Better at CP in 2 Months** curriculum — the
same 8-week plan from [`README.md`](README.md), with click-to-check progress,
filters, and search instead of manually editing markdown checkboxes on a branch.

115 items across 2 months: 78 problems, 27 reading resources, 10 contests.

![cp2go tracker](https://img.shields.io/badge/data-generated%20from-README-blue)

## Use it

Open `index.html` in a browser. That's it — no server, no build step, no
dependencies. Double-clicking the file works.

## What it does

- **Progress ring** — overall completion, split by problems / reading / contests.
- **Per-week sidebar** — every week with a live `done/total` count.
- **Filters** — by kind, by platform (Codeforces, LeetCode, HackerRank, CodeChef,
  SPOJ, HackerEarth, AtCoder, cp-algorithms, …), by difficulty (the ★ ratings).
- **Search** — titles, platforms, section names. Press <kbd>/</kbd> to focus, <kbd>Esc</kbd> to clear.
- **Difficulty stars** and **platform badges** on every row.
- **Video badges** — a `▶ video` link on the reading items that have a
  companion YouTube lecture.
- Progress saves to `localStorage` as you click. `Reset progress` clears it
  behind a confirm.

## Export / import

Progress lives in `localStorage`, which is **per-browser and per-origin** — it
doesn't follow you to another machine, and clearing site data loses it. Use
**Export** to download a `cp2go-progress-YYYY-MM-DD.json` file, and **Import**
to load one back. You can also just paste the JSON anywhere on the page.

```json
{
 "format": "cp2go-progress",
 "version": 1,
 "exported": "2026-10-03T07:21:37.823Z",
 "total": 115,
 "done": [
  { "id": "3f7200a32e", "t": "Dynamic Programming Notes Hackerearth",
    "u": "https://www.hackerearth.com/…", "m": 1, "w": 1 }
 ]
}
```

Two things worth knowing:

- **Files are portable across forks.** Item IDs are hashes of
  month + week + title + URL, so an ID from a fork whose curriculum has since
  changed won't match. The file also stores each item's title and URL, so the
  importer falls back to matching on those when the ID is unknown. Items that
  exist nowhere in your copy are reported as skipped rather than silently
  dropped.
- **Import replaces, it does not merge.** You'll see a breakdown of what
  matched before anything is overwritten. Export first if you want to keep the
  current state.

Because the file is plain JSON, you can commit it to git to keep progress
across machines, or keep one per phase of the plan.

## Keeping it in sync with upstream

`README.md` is the source of truth — this app does not hand-maintain a copy of
the curriculum. `build_data.py` parses it into `data.js`:

```bash
python3 build_data.py
```

```
wrote data.js: 2 months, 8 weeks, 115 items (115 checkboxes in README)
  problems=78 reading=27 contests=10
```

It is self-checking: it counts the checkboxes in the README and warns if the
parsed count doesn't match, so a silent upstream restructure can't quietly drop
problems. Run it after pulling upstream changes and commit the regenerated
`data.js`.

Item IDs are hashed from month + week + title + URL, so **your saved progress
survives upstream edits that only reorder or insert items.**

## Files

| File | Purpose |
|------|---------|
| `index.html` | The app. Static, self-contained, no dependencies. |
| `data.js` | Generated curriculum data. Do not hand-edit. |
| `build_data.py` | README → `data.js` parser and validator. |
| `README.md` | The upstream curriculum. Untouched. |

## Credits

Curriculum by [sahilbansal17](https://github.com/sahilbansal17/Get_Better_at_CP_in_2_Months).
This repo is a fork with the tracker added; the plan itself is unchanged.