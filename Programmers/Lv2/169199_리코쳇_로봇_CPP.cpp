#include <string>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int solution(vector<string> board) {
    int answer = 0;
    int stx = 0, sty = 0;
    int cnt = 0;
    queue<pair<int, int>> q;
    
    // cout << board.size();
    vector<vector<int>> bd(board.size()+2, vector<int>(board[0].size()+2, 1));
    vector<vector<bool>> vis(board.size()+2, vector<bool>(board[0].size()+2));
    vector<vector<int>> dist(board.size()+2, vector<int>(board[0].size()+2));
    
    for(int i = 0; i < board.size()+2; i++){
        for(int j = 0; j < board[0].size() + 2; j++){
            if(j == 0 || i == 0 || i == board.size()+1|| j == board[0].size()+1){
                bd[i][j] = 0;
            }
        }
    }
    
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[0].size(); j++){
            if(board[i][j] == 'D'){
                bd[i+1][j+1] = 0;
            }else if(board[i][j] == 'R'){
                stx = j+1;
                sty = i+1;
            }else if(board[i][j] == 'G'){
                bd[i+1][j+1] = 2;
            }
        }
    }
    vis[sty][stx] = 1;
    for(auto ar : vis){
        for(auto a : ar){
            cout << a << ' ';
        }
        cout << '\n';
    }
    q.push({sty, stx});
    
    while(!q.empty()){
        auto cur = q.front(); q.pop();
        int curx = cur.second;
        int cury = cur.first;
        for(int i = 0; i < 4; i++){
            int nx = curx + dx[i];
            int ny = cury + dy[i];
            if(bd[ny][nx] == 0) continue;
            
            while(bd[ny][nx] != 0){
                nx += dx[i];
                ny += dy[i];
            }
            ny -= dy[i];
            nx -= dx[i];
            if(vis[ny][nx] == 1) continue;
            vis[ny][nx] = 1;
            dist[ny][nx] = dist[cury][curx] + 1;
            if(bd[ny][nx] == 2) return dist[ny][nx];
            q.push({ny, nx});
            // cnt++;
        }
    }
    cout << cnt;
    
    return -1;
}