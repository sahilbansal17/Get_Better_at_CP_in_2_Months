#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 1000000007;

int q, n, f[8200] = {0};
ll dp[1005][8200] = {0};
ll cnt[1005] = {0};
vector<int> primes;

void sieve() {
  f[0] = f[1] = 1;
  for (int i = 2; i * i < 8200; i++) {
    if (!f[i]) {
      for (int j = i + i; j < 8200; j += i) {
          f[j] = 1;
      }
    }
  }
  for (int i = 2; i < 8200; i++) {
    if (!f[i]) {
        primes.push_back(i);
    }
  }
}

int main() {
  cin >> q;
  sieve();

  while (q--) {
    cin >> n;
    memset(cnt, 0, sizeof(cnt));
    memset(dp, 0, sizeof(dp));

    int x;
    for (int i = 0; i < n; i++) {
        cin >> x;
        cnt[x - 3500]++;
    }

    /*

    Reference:
    https://www.hackerrank.com/challenges/prime-xor/forum/comments/327009

    DP(i, j) := The # of subset using numbers within [3500, 3500+i] whose XOR
    result of the subset equals to j (3500 <= i <= 4500) 
    Now, DP(i, j) should consists of two disjoint parts: 
    1. DP(i-1, j) which means i has no effect at all on the XOR result 
    2. DP(i-1, j^i) which means i has effect at all on the
    XOR result 

    So Part 1 includes subsets of DP(i-1, j) union even number of i,
    as even number of i will produce 0 on the XOR result (no effect) 

    Part 2 on the contrast includes subsets of DP(i-1, j^i) union odd number of i, as odd
    number of i will produce a XOR i effect
    */

    dp[0][3500] = (cnt[0] + 1) / 2;
    dp[0][0] = (cnt[0] + 2) / 2;
    for (int i = 1; i < 1005; i++) {
      for (int j = 0; j < 8200; j++) {
        dp[i][j] = (dp[i - 1][j] * ((cnt[i] + 2) / 2) +
                    dp[i - 1][j ^ (i + 3500)] * ((cnt[i] + 1) / 2)) %
                   MOD;
      }
    }
    ll ans = 0;
    for (int p : primes) {
      (ans += dp[1004][p]) %= MOD;
    }
    cout << ans % MOD << endl;
  }
  return 0;
}