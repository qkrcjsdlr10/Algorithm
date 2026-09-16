#include <bits/stdc++.h>
using namespace std;

string grid[100];
int max_len; // 최대 팰린드롬 길이

// 가로 방향 DP
void check_row(int row) {
    int n = 100;
    bool dp[n][n];
    fill(&dp[0][0], &dp[0][0] + n*n, false);

    // 길이 1
    for (int i = 0; i < n; i++) dp[i][i] = true;

    // 길이 2
    for (int i = 0; i < n-1; i++) {
        if (grid[row][i] == grid[row][i+1]) {
            dp[i][i+1] = true;
            max_len = max(max_len, 2);
        }
    }

    // 길이 3 이상
    for (int len = 3; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            if (grid[row][i] == grid[row][j] && dp[i+1][j-1]) {
                dp[i][j] = true;
                max_len = max(max_len, len);
            }
        }
    }
}

// 세로 방향 DP (열 단위)
void check_col(int col) {
    int n = 100;
    bool dp[n][n];
    fill(&dp[0][0], &dp[0][0] + n*n, false);

    // 길이 1
    for (int i = 0; i < n; i++) dp[i][i] = true;

    // 길이 2
    for (int i = 0; i < n-1; i++) {
        if (grid[i][col] == grid[i+1][col]) {
            dp[i][i+1] = true;
            max_len = max(max_len, 2);
        }
    }

    // 길이 3 이상
    for (int len = 3; len <= n; len++) {
        for (int i = 0; i <= n - len; i++) {
            int j = i + len - 1;
            if (grid[i][col] == grid[j][col] && dp[i+1][j-1]) {
                dp[i][j] = true;
                max_len = max(max_len, len);
            }
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    for (int tc = 1; tc <= 10; tc++) {
        int test_case;
        cin >> test_case;

        max_len = 1; // 최소 길이 1로 초기화

        // 입력
        for (int i = 0; i < 100; i++) cin >> grid[i];

        // 가로/세로 방향 검사
        for (int i = 0; i < 100; i++) {
            check_row(i); // 각 행 검사
            check_col(i); // 각 열 검사
        }

        cout << '#' << test_case << ' ' << max_len << '\n';
    }

    return 0;
}
