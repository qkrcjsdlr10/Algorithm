#include <string>
#include <vector>
#include <queue>
using namespace std;

int solution(int n, vector<vector<int>> computers) {
    int answer = 0;
    vector<int> vis(n, 0);

    for (int i = 0; i < n; i++) {
        if (vis[i]) continue;

        queue<int> q;
        q.push(i);
        vis[i] = 1;

        while (!q.empty()) {
            int cur = q.front();
            q.pop();

            for (int next = 0; next < n; next++) {
                if (computers[cur][next] == 0) continue;
                if (vis[next]) continue;

                vis[next] = 1;
                q.push(next);
            }
        }

        answer++;
    }

    return answer;
}