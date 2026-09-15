#include<vector>
#include<queue>
#include<iostream>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int solution(vector<vector<int> > maps)
{
    int answer = 0;
    vector<vector<int>> board;
    board.resize(maps.size(), vector<int> (maps[0].size()));
    queue<pair<int, int>> q;
    // cout << board.size() << maps.size();
    q.push({0, 0});
    board[0][0] = 1;
    
    while(!q.empty()){
        for(int i = 0; i < 4; i++){
            int ny = q.front().first + dy[i];
            int nx = q.front().second + dx[i];
            // cout << ny << nx << '\n';
            if(nx < 0 || nx >= maps[0].size() || ny < 0 || ny >= maps.size()) continue;
            if(maps[ny][nx] == 0 || board[ny][nx] != 0) continue;
            // cout << ny << nx << ' ';
            q.push({ny, nx});
            board[ny][nx] = board[q.front().first][q.front().second]+1;
            // q.pop();
            // cout << ny << nx << ' ';
        }
        q.pop();
    }
    
    // for(auto a : board){
    //     for(auto b : a){
    //         cout << b << ' ';
    //     }
    //     cout << '\n';
    // }
    
    if(board[maps.size()-1][maps[0].size()-1] == 0){
        answer = -1;
    }else{
        answer = board[maps.size()-1][maps[0].size()-1];
    }
    
    return answer;
}