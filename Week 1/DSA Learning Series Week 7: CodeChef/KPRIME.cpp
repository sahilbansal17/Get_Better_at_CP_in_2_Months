#include <bits/stdc++.h>
using namespace std;
#define FAST_IO ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
typedef long long ll;

const int MAXN = 1e5 + 10;

bool prime[MAXN];
vector<int> primes;

// populate the list of primes below MAXN using sieve of eratosthenes
void sieve() {
    memset(prime, true, sizeof(prime));
    for (ll i = 2; i < MAXN; ++i) {
        if (!prime[i]) {
            continue;
        }
        primes.push_back(i);
        for (ll j = i*i; j < MAXN; j += i) {
            prime[j] = false;  
        }
    }
}

// count the no of prime factors in a number in O(sqrt(N)) using this populated list of primes
int count_pf(int x) {
    int k = 0;
    int res = 0;
    int maxk = sqrt(x);
    while (x > 1 && k < primes.size()) {
        int cur = primes[k];
        if (cur > maxk) {
            break;
        }
        if (x % cur == 0) {
            ++res;
        }
        while (x % cur == 0) {
            x /= cur;
        }
        ++k;
    }
    // left over must be prime
    if (x > 1) {
        ++res;
    }
    return res;
}

// count the prefix sum of k primes with 1 <= k <= 5 from 1 to MAXN
// pref_k_prime[k][x] denotes no of k primes from 1 to x
vector<vector<int> > pref_k_prime(6, vector<int>(MAXN, 0));

void solve() {
    int a, b, k;
    cin >> a >> b >> k;

    // no of k primes from a - b
    cout << pref_k_prime[k][b] - pref_k_prime[k][a - 1] << endl;
}

int main() {
    FAST_IO;

    int t;
    cin >> t;

    // call sieve
    sieve();

    // count pf and mark 1 at locations with k prime
    for (int i = 2; i <= 1e5; ++i) {
        int x = count_pf(i);
        if (x <= 5) {
            pref_k_prime[x][i] += 1;
        }
    }

    // populate prefix sum
    for (int i = 1; i <= 5; ++i) {
        for (int j = 1; j <= 1e5; ++j) {
            pref_k_prime[i][j] += pref_k_prime[i][j - 1];
        }
    }

    while (t--) {
        solve();
    }
    return 0;
}