#include <string>
#include <vector>
using namespace std;

int N;
int answer;
vector<vector<int>> attack;

void mark(int row, int col, int value) {
    // 같은 열
    for (int r = 0; r < N; r++) {
        attack[r][col] += value;
    }

    // 같은 행
    for (int c = 0; c < N; c++) {
        attack[row][c] += value;
    }

    // 왼쪽 위 대각선
    for (int r = row, c = col; r >= 0 && c >= 0; r--, c--) {
        attack[r][c] += value;
    }

    // 오른쪽 위 대각선
    for (int r = row, c = col; r >= 0 && c < N; r--, c++) {
        attack[r][c] += value;
    }

    // 왼쪽 아래 대각선
    for (int r = row, c = col; r < N && c >= 0; r++, c--) {
        attack[r][c] += value;
    }

    // 오른쪽 아래 대각선
    for (int r = row, c = col; r < N && c < N; r++, c++) {
        attack[r][c] += value;
    }
}

void dfs(int row) {
    if (row == N) {
        answer++;
        return;
    }

    for (int col = 0; col < N; col++) {
        // 공격받지 않는 칸이면 Queen 배치 가능
        if (attack[row][col] == 0) {
            mark(row, col, 1);      // Queen 놓고 공격 범위 표시

            dfs(row + 1);           // 다음 행 탐색

            mark(row, col, -1);     // Queen 제거, 원상복구
        }
    }
}

int solution(int n) {
    N = n;
    answer = 0;

    attack.assign(n, vector<int>(n, 0));

    dfs(0);

    return answer;
}