#include <string>
#include <vector>

using namespace std;

void calculatediff(vector<vector<int>> &diff, int type, int y1, int x1, int y2, int x2, int degree){
    
    if(type == 1) degree *= -1;
    
    diff[y1][x1] += degree;
    diff[y1][x2 + 1] += -1 * degree;
    diff[y2 + 1][x1] += -1 * degree;
    diff[y2 + 1][x2 + 1] += degree;
    
}


int solution(vector<vector<int>> board, vector<vector<int>> skill) {
    int answer = 0;
    
    int n = board.size();
    int m = board[0].size();
    
    vector<vector<int>> diff(n + 1, vector<int> (m + 1));
    
    for(auto sk : skill){
        calculatediff(diff, sk[0], sk[1], sk[2], sk[3], sk[4], sk[5]);
    }
    
    for(int i = 0; i < n + 1; i++){
        for(int j = 1; j < m + 1; j++){
            diff[i][j] += diff[i][j - 1];
        }
    }
    for(int i = 0; i < m + 1; i++){
        for(int j = 1; j < n + 1; j++){
            diff[j][i] += diff[j - 1][i];
        }
    }
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            board[i][j] += diff[i][j];
        }
    }
    
    for(auto ar : board){
        for(auto a : ar){
            if(a > 0) answer++;
        }
    }
    
    return answer;
}