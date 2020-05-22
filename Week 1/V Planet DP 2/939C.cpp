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

int main() {
    FAST_IO;

    cin >> n;
    readArray();

    string s;
    cin >> s;

    vector<int> cnt(n, 0);
    if (s[0] == '1') {
        cnt[1] = 1;
    }

    for (int i = 1; i < n - 1; ++i) {
        cnt[i + 1] = (s[i] == '1') ? cnt[i] + 1 : cnt[i];
    }

    for (int i = 0; i < n; ++i) {
        if (arr[i] < i + 1) {
            // all swaps must be allowed from arr[i] - 1 to i - 1
            if (cnt[i] - cnt[arr[i] - 1] != i - arr[i] + 1) {
                // cout << i + 1 << endl;
                cout << "NO" << endl;
                return 0;
            }
        } else if (arr[i] > i + 1) {
            // all swaps must be allowed from i to arr[i] - 2
            if (cnt[arr[i] - 1] - cnt[i] != arr[i] - i - 1) {
                // cout << i + 1 << endl;
                cout << "NO" << endl;
                return 0;
            }
        }
    }
    cout << "YES" << endl;
    return 0;
}