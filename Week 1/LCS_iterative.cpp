/*
 * Problem Link: https://www.hackerrank.com/challenges/dynamic-programming-classics-the-longest-common-subsequence/problem
 */
#include <bits/stdc++.h>
using namespace std;

vector<int> longestCommonSubsequence(vector<int> a, vector<int> b) {

    int n = a.size();
    int m = b.size();
    vector<vector<int>> lcs(n, vector<int>(m, 0));
    // lcs[i][j] denotes lcs length for a[0...i] and b[0...j]

    /* lcs[i][j] = (lcs[i - 1], [j - 1] + 1, if a[i] == b[j],
                    max(lcs[i - 1][j], lcs[i][j - 1] otherwise )
    */

    for (int i = 0; i < m; ++i) {
        if (a[0] == b[i]) {
            lcs[0][i] = 1;
        } else if (i != 0) {
            lcs[0][i] = lcs[0][i - 1];
        }
    }
    for (int i = 0; i < n; ++i) {
        if (a[i] == b[0]) {
            lcs[i][0] = 1;
        } else if (i != 0) {
            lcs[i][0] = lcs[i - 1][0];
        }
    }

    for (int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            if (a[i] == b[j]) {
                lcs[i][j] = lcs[i - 1][j - 1] + 1;
            } else {
                lcs[i][j] = max(lcs[i - 1][j], lcs[i][j - 1]);
            }
        }
    }
    
    vector<int> res;
    int i = n - 1;
    int j = m - 1;
    while (i >= 0 && j >= 0) {
        if (a[i] == b[j]) {
            res.push_back(a[i]);
            --i;
            --j;
        } else {
            int opt1 = (i - 1 >= 0) ? lcs[i - 1][j] : 0;
            int opt2 = (j - 1 >= 0) ? lcs[i][j - 1] : 0;
            if (opt1 > opt2) {
                --i;
            } else {
                --j;
            }
        }
    }
    reverse(res.begin(), res.end());
    return res;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<int> a(n), b(m);
    for (auto &x: a) {
        cin >> x;
    }
    for (auto &y: b) {
        cin >> y;
    }
    vector<int> res = longestCommonSubsequence(a, b);
    for (auto z: res) {
        cout << z << " ";
    }
    cout << endl;
    return 0;
}