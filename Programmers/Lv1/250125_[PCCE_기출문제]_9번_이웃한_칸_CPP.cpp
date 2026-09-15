#include <string>
#include <vector>
#include <iostream>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int solution(vector<vector<string>> board, int h, int w) {
    int answer = 0;
    int n = board.size(), m = board[0].size();
    
    for(int i = 0; i < 4; i++){
        int nx = w + dx[i];
        int ny = h + dy[i];
        if(nx >= m || nx < 0 || ny >= n || ny < 0) continue;
        // cout << board[ny][nx] << ' ';
        if(board[h][w] == board[ny][nx]) answer++;
    }
    
    
    return answer;
}