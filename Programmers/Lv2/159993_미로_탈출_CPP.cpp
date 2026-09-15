#include <string>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int solution(vector<string> maps) {
    int answer = 0;
    int n = maps.size(), m = maps[0].size();
    int sx = 0, sy = 0;
    int tx = 0, ty = 0;
    int lx = 0, ly = 0;
    int cnt = 0;
    vector<vector<int>> vis(n, vector<int>(m, -1));
    
    queue<pair<int, int>> q;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            // cout << maps[i][j] << ' ';
            if(maps[i][j] == 'S'){
                sx = j;
                sy = i;
                vis[i][j] = 0;
            }else if(maps[i][j] == 'E'){
                tx = j;
                ty = i;
            }else if(maps[i][j] == 'L'){
                lx = j;
                ly = i;
            }
        }
        cout << '\n';
    }
    // cout << sy << ' ' << sx;
    
    q.push({sy, sx});
    
    while(!q.empty()){
        int y = q.front().first;
        int x = q.front().second;
        q.pop();
        
        for(int i = 0; i < 4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if(nx >= m || nx < 0 || ny >= n || ny < 0) continue;
            if(maps[ny][nx] == 'X' || vis[ny][nx] != -1) continue;
            if(maps[ny][nx] == 'L'){
                vis[ny][nx] = vis[y][x] + 1;
                answer += vis[ny][nx];
                while(!q.empty()) q.pop();
                cout << answer;
                break;
            }
            q.push({ny, nx});
            vis[ny][nx] = vis[y][x] + 1;
        }
    }
    
    while(!q.empty()) q.pop();
    
    if(vis[ly][lx] != -1){
        q.push({ly, lx});
        vis.assign(n, vector<int>(m, -1));
        vis[ly][lx] = 0;
    }
    
    while(!q.empty()){
        int y = q.front().first;
        int x = q.front().second;
        q.pop();
        
        for(int i = 0; i < 4; i++){
            int nx = x + dx[i];
            int ny = y + dy[i];
            
            if(nx >= m || nx < 0 || ny >= n || ny < 0) continue;
            if(maps[ny][nx] == 'X' || vis[ny][nx] != -1) continue;
            q.push({ny, nx});
            vis[ny][nx] = vis[y][x] + 1;
            cnt++;
        }
    }
    
    // cout << vis[sy][sx];
    
    for(auto mm : vis){
        for(auto m : mm){
            cout << m << ' ';
        }
        cout << '\n';
    }
    
    if(vis[ty][tx] != -1 && vis[ly][lx] != -1){
        answer += vis[ty][tx];
    }else{
        answer = -1;
    }
    
    return answer;
}