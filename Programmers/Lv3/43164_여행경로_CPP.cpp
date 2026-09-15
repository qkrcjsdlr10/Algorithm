#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int n;
vector<bool> used;
vector<string> path, answer;
bool found = false;

void dfs(string current, vector<vector<string>>& tickets, int depth) {
    if (found) return;

    if (depth == n) {
        answer = path;
        found = true;
        return;
    }

    for (int i = 0; i < n; i++) {
        if (used[i]) continue;
        if (tickets[i][0] != current) continue;

        used[i] = true;
        path.push_back(tickets[i][1]);

        dfs(tickets[i][1], tickets, depth + 1);

        used[i] = false;
        path.pop_back();
    }
}

vector<string> solution(vector<vector<string>> tickets) {
    n = tickets.size();
    sort(tickets.begin(), tickets.end());
    used.resize(n, false);

    path.push_back("ICN");
    dfs("ICN", tickets, 0);

    return answer;
}