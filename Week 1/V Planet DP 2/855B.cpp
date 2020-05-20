#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;

int n;
ll p, q, r;
vector<ll> arr;

void readArray() {
    arr.resize(n);
    for (auto &a: arr) {
        cin >> a;
    }
}

ll bestans[4][100010];

ll solve(int curPick, int cnt, int id) {
    if (id == n) {
        if (curPick == 3 && cnt == 3) {
            return 0;
        }
        return LLONG_MIN;
    }
    if (bestans[curPick][id] != -1) {
        return bestans[curPick][id];
    }
    // don't pick
    ll ans = solve(curPick, cnt, id + 1);
    // pick
    if (curPick == 0) {
        ans = max(ans, p*arr[id] + solve(curPick + 1, cnt + 1, id));
    } else if (curPick == 1) {
        ans = max(ans, q*arr[id] + solve(curPick + 1, cnt + 1, id));
    } else if (curPick == 2) {
        ans = max(ans,r*arr[id] + solve(curPick + 1, cnt + 1, id));
    } else {
        // already picked all
        ans = max(ans, solve(curPick, cnt, id + 1));
    }
    return bestans[curPick][id] = ans;
}

int main() {
    FAST_IO;

    cin >> n >> p >> q >> r;

    readArray();
    memset(bestans, -1, sizeof(bestans));
    cout << solve(0, 0, 0) << endl;

    /*/
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << bestans[i][j] << " ";
        }
        cout << endl;
    }
    //*/
    return 0;
}