#include <bits/stdc++.h>
using namespace std;

#define X first
#define Y second

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int v, e, st;
    cin >> v >> e >> st;

    const int INF = 1e9;
    vector<int> d(20005, INF);
    vector<pair<int,int>> adj[20005];

    for (int i = 0; i < e; i++) {
        int u, to, w;
        cin >> u >> to >> w;
        adj[u].push_back({w, to});
    }

    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    d[st] = 0;
    pq.push({0, st});

    while (!pq.empty()) {
        auto cur = pq.top(); pq.pop();
        int dist = cur.X;
        int node = cur.Y;

        if (d[node] != dist) continue;

        for (auto nxt : adj[node]) {
            int w = nxt.X;
            int to = nxt.Y;
            if (d[to] > dist + w) {
                d[to] = dist + w;
                pq.push({d[to], to});
            }
        }
    }

    for (int i = 1; i <= v; i++) {
        if (d[i] == INF) cout << "INF\n";
        else cout << d[i] << "\n";
    }
}