#include <string>
#include <vector>

using namespace std;

int solution(int m, int n, vector<vector<int>> puddles) {
    const int MOD = 1000000007;
    vector<vector<int>> dp(n, vector<int>(m, 0));

    for (auto &p : puddles) {
        dp[p[1] - 1][p[0] - 1] = -1;
    }

    dp[0][0] = 1;

    for (int y = 0; y < n; y++) {
        for (int x = 0; x < m; x++) {
            if (dp[y][x] == -1) continue;
            if (y == 0 && x == 0) continue;

            long long up = 0, left = 0;

            if (y > 0 && dp[y - 1][x] != -1) up = dp[y - 1][x];
            if (x > 0 && dp[y][x - 1] != -1) left = dp[y][x - 1];

            dp[y][x] = (up + left) % MOD;
        }
    }

    return dp[n - 1][m - 1];
}