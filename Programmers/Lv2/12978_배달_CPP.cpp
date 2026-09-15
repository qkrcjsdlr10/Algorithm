#include <bits/stdc++.h>
using namespace std;

int solution(int N, vector<vector<int>> road, int K) {
    const int INF = 1e9;
    vector<vector<pair<int,int>>> adj(N+1); // {가중치, 다음정점}
    
    for (auto &r : road) {
        int u = r[0], v = r[1], w = r[2];
        adj[u].push_back({w, v});
        adj[v].push_back({w, u}); // 무방향
    }

    vector<int> d(N+1, INF);
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    d[1] = 0;
    pq.push({0, 1}); // {거리, 정점}

    while (!pq.empty()) {
        auto [dist, v] = pq.top(); pq.pop();
        if (d[v] != dist) continue;

        for (auto [w, nv] : adj[v]) {
            if (d[nv] > dist + w) {
                d[nv] = dist + w;
                pq.push({d[nv], nv});
            }
        }
    }

    int answer = 0;
    for (int i = 1; i <= N; i++) {
        if (d[i] <= K) answer++;
    }
    return answer;
}