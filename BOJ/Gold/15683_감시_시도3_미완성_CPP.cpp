#include <bits/stdc++.h>
using namespace std;

int n, m;
int arr[100][100];
bool vis[100][100]; // 방문 여부 확인
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};
int maxCnt = 0; // 최대값 저장

// DFS 함수
int dfs(int x, int y) {
    // 경계 및 벽 검사
    if (x < 0 || x >= n || y < 0 || y >= m || arr[x][y] == 6 || vis[x][y]) {
        return 0;
    }

    // 방문 처리
    vis[x][y] = true;

    int cnt = 1; // 현재 위치 포함

    // 네 방향으로 탐색
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        cnt += dfs(nx, ny); // 재귀 호출로 연결된 영역 크기 계산
    }

    return cnt;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m;

    queue<pair<int, int>> Q;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cin >> arr[i][j];
            if (arr[i][j] == 1) {
                Q.push({i, j}); // 시작점 저장
            }
        }
    }

    while (!Q.empty()) {
        auto cur = Q.front();
        Q.pop();

        // 방문하지 않은 영역에 대해 DFS 수행
        if (!vis[cur.first][cur.second]) {
            int cnt = dfs(cur.first, cur.second);
            maxCnt = max(maxCnt, cnt); // 최대값 갱신
        }
    }

    cout << maxCnt << '\n'; // 최대값 출력

    return 0;
}
