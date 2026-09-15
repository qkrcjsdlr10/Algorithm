#include <vector>
#include <algorithm>
using namespace std;

int solution(vector<vector<int>> board) {
    int n = board.size();
    int m = board[0].size();
    int answer = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (board[i][j] == 1) {
                if (i > 0 && j > 0) {
                    board[i][j] = min({
                        board[i-1][j],     // 위
                        board[i][j-1],     // 왼쪽
                        board[i-1][j-1]    // 왼쪽 위
                    }) + 1;
                }
                answer = max(answer, board[i][j]);
            }
        }
    }

    return answer * answer;
}