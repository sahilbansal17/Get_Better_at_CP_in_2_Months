#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;

const ll MOD = 1e9 + 9;
const int MAXN = 1e6 + 10;

ll numWays[MAXN];

ll solve(ll x) {
    if (x < 2) {
        return 0;
    }
    if (x == 2 || x == 3) {
        return numWays[x] = 1;
    }
    if (numWays[x] != -1) {
        return numWays[x];
    }
    return numWays[x] = (solve(x - 2) + solve(x - 3)) % MOD;
}
void solve() {
    ll x;
    cin >> x;

    cout << solve(x) << endl;
}

int main() {
    FAST_IO;

    int t;
    cin >> t;

    memset(numWays, -1, sizeof(numWays));
    while (t--) {
        solve();
    }
    return 0;
}