#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;
#define endl '\n'

void solve() {
    int n;
    cin >> n;

    vector<ll> arr(n);
    for (auto &a: arr) {
        cin >> a;
    }

    // arr = [1, 4, 2, 3, 1]
    vector<ll> diff(n - 1); // a[i] - a[i + 1] = diff[i]
    for (int i = 0; i < n - 1; ++i) {
        diff[i] = abs(arr[i] - arr[i + 1]);
    }

    // diff = [3, 2, 1, 2]
    ll ans = -1e18;
    ll best0 = 0, best1 = -1e18;
    ll curSum = 0;
    for (int i = 0; i < n - 1; ++i) {
        curSum += ((i % 2 == 0) ? diff[i] : -diff[i]);//|   3   1   2   0 
        ans = max(ans, curSum - best0);               //|   3   3   3   3
        ans = max(ans, best1 - curSum);               //|   3   3   3   3
        if (i % 2 == 0) {                             //|
            best1 = max(best1, curSum);               //|   3   3   3   3
        } else {                                      //|  
            best0 = min(best0, curSum);               //|   0   0   0   0
        }
    }
    cout << ans << endl; // 3
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