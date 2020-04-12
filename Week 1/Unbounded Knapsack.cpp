/*
 * Problem Link: https://www.hackerrank.com/challenges/unbounded-knapsack/problem
 */

#include <bits/stdc++.h>
using namespace std;

int ans = INT_MIN;
map<string, int> done;
void unboundedKnapsack(int k, vector<int> arr, int idx, int sum) {
    if (sum > k) {
        return ;
    }
    if (idx == arr.size() ) {
        if (sum > ans) {
            ans = sum;
        }
        return ;
    }
    string key = to_string(sum) + "-" + to_string(idx);
    if (done[key]) {
        return ;
    }
    // pick current item while cur_sum doesn't exceed k, also don't pick the item
    int cur_sum = sum;
    while (cur_sum <= k) {
        unboundedKnapsack(k, arr, idx + 1, cur_sum);
        cur_sum += arr[idx];
    }
    done[key] = 1;
    return;
}

void solve() {
    int n, k;
    cin >> n >> k;
    vector<int> arr(n);
    for (auto &x: arr) {
        cin >> x;
    }
    ans = INT_MIN;
    done.clear();
    unboundedKnapsack(k, arr, 0, 0);
    cout << ans << endl;
}
int main() {
   int t;
   cin >> t;

   while (t--) {
       solve();
   }
}