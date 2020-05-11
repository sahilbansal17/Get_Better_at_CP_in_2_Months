#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;
#define endl '\n'

int done[110][8];
int rec_solve(int cur, int rem, string& s, string num, string& res) {
    if (rem == 0 && num.length() > 0) {
        res = num;
        return 1;
    }
    if (cur == s.length()) {
        if (rem == 0 && num.length() > 0) {
            res = num;
            return 1;
        }
        return 0;
    }
    if (done[cur][rem] != -1) {
        return done[cur][rem];
    }
    // don't add the cur digit
    int dontadd = rec_solve(cur + 1, rem, s, num, res);
    // add the cur digit
    int dig = s[cur] - '0';
    char ch = s[cur];
    int add = rec_solve(cur + 1, ((rem*10)%8 + dig%8)%8, s, num + ch, res);
    if (add == 1 || dontadd == 1) {
        return done[cur][rem] = 1;
    }
    return done[cur][rem] = 0;
}

void solve() {
    string s;
    cin >> s;

    int n = s.length();
    string res = "";
    
    memset(done, -1, sizeof(done));

    int checker = rec_solve(0, 0, s, "", res);
    if (res.length() > 0) {
        cout << "YES" << endl;
        cout << res << endl;
    } else {
        cout << "NO" << endl;
    }
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