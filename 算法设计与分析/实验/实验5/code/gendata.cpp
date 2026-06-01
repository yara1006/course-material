#include <bits/stdc++.h>
using namespace std;

int n = 1e7, m = 2e7;
set<pair<int, int>> edges;

int main() {
    freopen("1e7_2e7.txt", "w", stdout);
    mt19937 rng(time(0));
    uniform_int_distribution<int> dist(1, n);

    while ((int)edges.size() < m) {
        int u = dist(rng), v = dist(rng);
        if (u == v) continue;
        if (u > v) swap(u, v);
        edges.insert({u, v}); // set 自动去重
    }

    printf("%d %d\n", n, m);
    for (auto [u, v] : edges) {
        printf("%d %d\n", u, v);
    }
    return 0;
}
