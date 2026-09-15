#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int solution(vector<vector<int>> board) {
    int answer = 0;
    
    int n = board.size();
    vector<vector<vector<int>>> cost(
        n, vector<vector<int>>(
        n, vector<int>(4, 1e9))
    );
    queue<tuple<int, int, int>> q;
    q.push({0, 0, -1});
    
    while(!q.empty()){
        auto [y, x, dir] = q.front(); q.pop();
        
        int curcost = (dir == -1 ? 0 : cost[y][x][dir]);
        
        for(int d = 0; d < 4; d++){
            int nx = x + dx[d];
            int ny = y + dy[d];
            int newcost = 0;
            
            if(dir == -1) newcost = 100;
            else if(dir == d) newcost = curcost + 100;
            else newcost = curcost + 600;
            
            if(nx >= n || nx < 0 || ny >= n || ny < 0) continue;
            if(board[ny][nx]) continue;
            if(nx == 0 && ny == 0) continue;
            if(cost[ny][nx][d] <= newcost) continue;
            
            cost[ny][nx][d] = newcost;
            
            q.push({ny, nx, d});
        }
        
    }
    
    answer =  *min_element(cost[n-1][n-1].begin(), cost[n-1][n-1].end());
    
    return answer;
}