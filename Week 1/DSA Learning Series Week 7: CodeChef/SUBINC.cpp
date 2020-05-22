#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;

int n;
vector<ll> arr;

void readArray() {
    arr.resize(n);
    for (auto &a: arr) {
        cin >> a;
    }
}

void solve() {
    cin >> n;
    readArray();

    vector<ll> cnt(n, 1);
    ll res = 1;
    for (int i = 1; i < n; ++i) {
        if (arr[i] >= arr[i - 1]) {
            cnt[i] += cnt[i - 1];
        }
        res += cnt[i];
    }
    cout << res << endl;
}

int main() {
    FAST_IO;

    int t;
    cin >> t;

    while (t--) {
        solve();
    }
    return 0;
}