#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;
vector<vector<pair<int, int>>> graph;

vector<int> dijkstra(int start, int n) {
    vector<int> dist(n + 1, INF);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[start] = 0;
    pq.push({0, start});

    while (!pq.empty()) {
        int curDist = pq.top().first;
        int curV = pq.top().second;
        pq.pop();

        if (curDist > dist[curV]) continue;

        for (auto nxt : graph[curV]) {
            int nextV = nxt.first;
            int cost = nxt.second;

            if (dist[nextV] > curDist + cost) {
                dist[nextV] = curDist + cost;
                pq.push({dist[nextV], nextV});
            }
        }
    }

    return dist;
}

int solution(int n, int s, int a, int b, vector<vector<int>> fares) {
    graph.assign(n + 1, vector<pair<int, int>>());

    for (auto fare : fares) {
        int u = fare[0];
        int v = fare[1];
        int w = fare[2];

        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
    }

    vector<int> distS = dijkstra(s, n);
    vector<int> distA = dijkstra(a, n);
    vector<int> distB = dijkstra(b, n);

    int answer = INF;

    for (int k = 1; k <= n; k++) {
        if (distS[k] == INF || distA[k] == INF || distB[k] == INF) continue;

        answer = min(answer, distS[k] + distA[k] + distB[k]);
    }

    return answer;
}