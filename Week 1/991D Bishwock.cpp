#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;
#define endl '\n'

void solve() {
    vector<string> board(2);
    cin >> board[0];
    cin >> board[1];

    int len = board[0].length();
    int prev = 0;
    int cur = 0;
    int ans = 0;
    for (int i = 0; i < len; ++i) {
        cur = (board[0][i] == '0') + (board[1][i] == '0');
        if (cur == 0) {
            prev = 0;
        }
        if (cur == 1) {
            if (prev == 2) {
                ++ans;
                prev = 0;
            } else {
                prev = 1;
            }
        }
        if (cur == 2) {
            if (prev > 0) {
                ++ans;
                if (prev == 2) {
                    prev = 1;
                } else {
                    prev = 0;
                }
            } else {
                prev = 2;
            }
        }
    }
    cout << ans << endl;
}

int main() {
    FAST_IO;

    int t;
    // cin >> t;
    t = 1;

    while (t--) {
        solve();
    }
    
    return 0;
}