#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

vector<vector<int>> board;
int mn;
void rotboard(int sy, int sx, int ey, int ex){
    int temp = board[sy][ex];
    mn = 1e9;
    mn = min(temp, mn);
    for(int i = ex; i > sx; i--){
        board[sy][i] = board[sy][i-1];
        mn = min(board[sy][i], mn);
    }
    
    int temp2 = board[ey][ex];
    mn = min(temp2, mn);
    
    for(int i = ey; i > sy; i--){
        board[i][ex] = board[i-1][ex];
        mn = min(board[i][ex], mn);
    }
    
    board[sy+1][ex] = temp;
    temp = board[ey][sx];
    mn = min(temp, mn);
    for(int i = sx; i < ex-1; i++){
        board[ey][i] = board[ey][i+1];
        mn = min(board[ey][i], mn);
    }
    board[ey][ex-1] = temp2;
    
    
    for(int i = sy; i < ey-1; i++){
        board[i][sx] = board[i+1][sx];
        mn = min(board[i][sx], mn);
    }
    board[ey-1][sx] = temp;
}

vector<int> solution(int rows, int columns, vector<vector<int>> queries) {
    vector<int> answer;
    
    board.resize(rows, vector<int>(columns));
    int a = 1;
    for(auto &ar : board){
        for(auto &b : ar){
            b = a;
            a++;
        }
    }
    for(int i = 0; i < queries.size(); i++){
        rotboard(queries[i][0]-1, queries[i][1]-1, queries[i][2]-1, queries[i][3]-1);
        answer.push_back(mn);
    }
    
    
    // for(auto ar : board){
    //     for(auto b : ar){
    //         cout << setw(2) << b << ' ';
    //     }
    //     cout << '\n';
    // }
    // cout << mn;
    
    return answer;
}