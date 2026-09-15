#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

vector<int> solution(vector<string> maps) {
    vector<int> answer;
    bool found = false;
    int n = maps.size();
    int m = maps[0].size();
    vector<vector<bool>> vis(n, vector<bool> (m, 0));
    queue<pair<int, int>> q;
    
    // q.push({0, 0});
    
    while(1){
        bool found = false;
        int sum = 0;
        for(int i = 0; i < n; i++){
            if(found) break;
            for(int j = 0; j < m; j++){
                if(maps[i][j] != 'X' && vis[i][j] == 0){
                    q.push({i, j});
                    vis[i][j] = 1;
                    sum += maps[i][j] - '0';
                    found = true;
                    break;
                }
            }
        }
        if(!found) break;
        
        while(!q.empty()){
            auto cur = q.front(); q.pop();
            int x = cur.second;
            int y = cur.first;
            int ny = 0;
            int nx = 0;

            for(int i = 0; i < 4; i++){
                nx = x + dx[i];
                ny = y + dy[i];

                if(nx >= m || nx < 0 || ny >= n || ny < 0) continue;
                if(maps[ny][nx] == 'X') continue;
                if(vis[ny][nx] == 1) continue;
                sum += maps[ny][nx] - '0';
                vis[ny][nx] = 1;
                q.push({ny, nx});
            }
        }
        answer.push_back(sum);
    }
    
    
    for(auto ar : vis){
        for(auto a : ar){
            cout << a;
        }
        cout << '\n';
    }
    
    sort(answer.begin(), answer.end());
    
    if(answer.size() == 0) answer.push_back(-1);
    return answer;
}