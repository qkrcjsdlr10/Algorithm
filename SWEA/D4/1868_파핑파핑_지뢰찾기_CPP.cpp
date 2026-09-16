#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

int dx[8] = {-1, 0, 1, 1, 1, 0, -1, -1};
int dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0};
vector<vector<char>> board;
int n = 0;

// 주변 8방향의 지뢰('*') 개수를 세는 함수
int findboom(int y, int x) {
    int boom = 0;
    for (int d = 0; d < 8; d++) {
        int nx = x + dx[d];
        int ny = y + dy[d];
        if (nx >= n || nx < 0 || ny < 0 || ny >= n) continue;

        if (board[ny][nx] == '*') {
            boom++;
        }
    }
    return boom;
}

// 지뢰 개수가 0인 칸을 클릭했을 때 주변 연쇄 오픈(BFS)
void clickZero(int y, int x) {
    queue<pair<int, int>> q;
    q.push({y, x});
    board[y][x] = '0'; // 방문 처리 (숫자로 변경)

    while (!q.empty()) {
        auto cur = q.front();
        q.pop();
        int cury = cur.first;
        int curx = cur.second;

        for (int d = 0; d < 8; d++) {
            int ny = cury + dy[d];
            int nx = curx + dx[d];

            if (nx >= n || nx < 0 || ny < 0 || ny >= n) continue;

            // 아직 방문하지 않은 빈 칸('.')인 경우
            if (board[ny][nx] == '.') {
                int boom = findboom(ny, nx);
                board[ny][nx] = boom + '0'; // 숫자 적어주고 방문 처리

                // 주변 지뢰도 0개라면 연쇄 탐색을 위해 큐에 추가
                if (boom == 0) {
                    q.push({ny, nx});
                }
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        cin >> n;

        board.assign(n, vector<char>(n, ' '));

        for (int i = 0; i < n; i++) {
            string temp = "";
            cin >> temp;
            for (int j = 0; j < n; j++) {
                board[i][j] = temp[j];
            }
        }

        int res = 0;

        // 1단계: 주변 지뢰가 0개인 칸을 '우선적으로' 찾아 연쇄적으로 터뜨림
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == '.' && findboom(i, j) == 0) {
                    res++;
                    clickZero(i, j);
                }
            }
        }

        // 2단계: 0에 의해 자동으로 열리지 못한 남은 빈 칸('.') 개수 더해주기
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board[i][j] == '.') {
                    res++;
                }
            }
        }

        // 클릭한 최소 횟수 출력
        cout << '#' << t << ' ' << res << '\n';
    }

    
    return 0;
}