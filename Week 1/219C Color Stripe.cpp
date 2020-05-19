#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;

int n, k;
string s;

int main() {
    FAST_IO;

    cin >> n >> k;
    cin >> s;

    if (k == 2) {
        int ans1 = 0;
        int ans2 = 0;
        // 'ABABAB...'
        // 'BABABA...'
        for (int i = 0; i < n; ++i) {
            if (i & 1) {
                if (s[i] == 'A') {
                    ++ans1;
                } else {
                    ++ans2;
                }
            } else {
                if (s[i] == 'B') {
                    ++ans1;
                } else {
                    ++ans2;
                }
            }
        }
        int res = min(ans1, ans2);
        cout << res << endl;
        if (res == ans1) {
            for (int i = 0; i < n; ++i) {
                if (i & 1) {
                    cout << 'B';
                } else {
                    cout << 'A';
                }
            }
        } else {
            for (int i = 0; i < n; ++i) {
                if (i & 1) {
                    cout << 'A';
                } else {
                    cout << 'B';
                }
            }
        }
    } else {
        /**
         * AABBB
         * ACBBB
         * ACBCB
         */
        int ans = 0;
        for (int i = 0; i < n - 1; ++i) {
            if (s[i] == s[i + 1]) {
                ++ans;
                if (i + 2 < n && s[i + 2] == s[i]) {
                    if (s[i] - 'A' + 1 < k) {
                        s[i + 1] = s[i] + 1;
                    } else {
                        s[i + 1] = s[i] - 1;
                    }
                } else {
                    if (i + 2 < n) {
                        int cur = s[i] - 'A' + 1;
                        int next = s[i + 2] - 'A' + 1;
                        for (int j = 1; j <= k; ++j) {
                            if (j != cur && j != next) {
                                s[i + 1] = 'A' + (j - 1);
                                break;
                            }
                        }
                    } else {
                        for (int j = 1; j <= k; ++j) {
                            if (s[i + 1] != ('A' + j - 1)) {
                                s[i + 1] = ('A' + j - 1);
                                break;
                            }
                        }
                    }
                }
            }
        }
        cout << ans << endl;
        cout << s << endl;
    }
    return 0;
}