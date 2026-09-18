#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> arr, board;

void rotate_layer(int n, int layer, int dir) {
    int start = layer;
    int end = n - 1 - layer;
    if (start >= end) return; // 중심에 도달하면 종료

    int mid = n / 2;

    // 시계 방향
    if (dir == 1) {
        for (int i = start; i <= end; i++) {
            // 위쪽 중앙 → 오른쪽 위 대각선
            board[i][end - i + layer] = arr[start][i];
            // 오른쪽 위 대각선 → 오른쪽 중앙열
            board[end - i + layer][end] = arr[i][end];
            // 오른쪽 중앙열 → 오른쪽 아래 대각선
            board[end][i] = arr[end - i + layer][end];
            // 오른쪽 아래 대각선 → 아래 중앙행
            board[end - i + layer][start] = arr[end][i];
            // 아래 중앙행 → 왼쪽 아래 대각선
            board[start][i] = arr[i][start];
        }
    }
    // 반시계 방향
    else {
        for (int i = start; i <= end; i++) {
            // 위쪽 중앙 → 왼쪽 위 대각선
            board[end - i + layer][start] = arr[start][i];
            // 왼쪽 위 대각선 → 왼쪽 중앙열
            board[i][start] = arr[end - i + layer][start];
            // 왼쪽 중앙열 → 왼쪽 아래 대각선
            board[end - i + layer][end] = arr[i][start];
            // 왼쪽 아래 대각선 → 아래 중앙행
            board[i][end] = arr[end][end - i + layer];
            // 아래 중앙행 → 오른쪽 아래 대각선
            board[start][i] = arr[i][end];
        }
    }

    // 안쪽 layer 재귀
    rotate_layer(n, layer + 1, dir);
}

void rotate45(int cnt, int n, int dir) {
    if (cnt == 0) return;
    board = arr; // 복사 초기화

    // 한 번의 45도 회전
    for (int i = 0; i < n; i++) {
        // 주대각선 → 중앙열
        board[i][n / 2] = arr[i][i];
        // 중앙열 → 부대각선
        board[i][n - 1 - i] = arr[i][n / 2];
        // 부대각선 → 중앙행
        board[n / 2][n - 1 - i] = arr[i][n - 1 - i];
        // 중앙행 → 주대각선
        board[i][i] = arr[n / 2][i];
    }

    arr = board; // 결과 반영
    rotate45(cnt - 1, n, dir); // cnt번 재귀 반복
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, angle;
    cin >> n >> angle;

    int cnt = ((angle % 360) + 360) % 360 / 45; // 회전 횟수
    int dir = (angle >= 0 ? 1 : -1); // 시계/반시계 방향

    arr.assign(n, vector<int>(n));
    board.assign(n, vector<int>(n));

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            cin >> arr[i][j];

    rotate45(cnt, n, dir);

    for (auto &row : arr) {
        for (auto &x : row)
            cout << x << ' ';
        cout << '\n';
    }

    return 0;
}
