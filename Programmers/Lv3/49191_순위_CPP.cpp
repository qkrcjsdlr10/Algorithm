#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> winGraph;
vector<vector<int>> loseGraph;
vector<bool> visited;

int dfs(int cur, vector<vector<int>>& graph) {
    int cnt = 0;

    for (int next : graph[cur]) {
        if (visited[next]) continue;

        visited[next] = true;
        cnt += 1 + dfs(next, graph);
    }

    return cnt;
}

int solution(int n, vector<vector<int>> results) {
    int answer = 0;

    winGraph.assign(n + 1, {});
    loseGraph.assign(n + 1, {});
    visited.assign(n + 1, false);

    for (auto result : results) {
        int winner = result[0];
        int loser = result[1];

        winGraph[winner].push_back(loser);
        loseGraph[loser].push_back(winner);
    }

    for (int player = 1; player <= n; player++) {
        fill(visited.begin(), visited.end(), false);
        visited[player] = true;
        int winCount = dfs(player, winGraph);

        fill(visited.begin(), visited.end(), false);
        visited[player] = true;
        int loseCount = dfs(player, loseGraph);

        if (winCount + loseCount == n - 1) {
            answer++;
        }
    }

    return answer;
}