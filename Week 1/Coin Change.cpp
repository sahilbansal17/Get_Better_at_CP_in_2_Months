#include <bits/stdc++.h>
using namespace std;

map<string, int> cache;
long getWays(int money, vector<long> coins, int idx) {
    if (money == 0) {
        return 1;
    }
    if (idx >= coins.size()) {
        return 0;
    }
    string key = to_string(money) + "-" + to_string(idx);
    if (cache.find(key) != cache.end()) {
        return cache[key];
    }
    int amountWithCoin = 0;
    long ways = 0;
    while (amountWithCoin <= money) {
        int remaining = money - amountWithCoin;
        ways += getWays(remaining, coins, idx + 1);
        amountWithCoin += coins[idx];
    }
    cache[key] = ways;
    return ways;
}

int main() {
    int n, m;
    cin >> n >> m;

    vector<long> coins;
    long c;
    for (int i = 0; i < m; ++i) {
        cin >> c;
        coins.push_back(c);
    }
    cout << getWays(n, coins, 0) << endl;
}
