#include <bits/stdc++.h>
using namespace std;


int dist[102][102];
int n,m;
int dx[] = {1, 0, -1, 0};
int dy[] = {0, 1, 0, -1};

string board[102];

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    queue<pair<int, int>> Q;

    cin >> n >> m;
    
    for(int i = 0; i < n; i++){
        cin >> board[i];
    }

    for(int i = 0; i < n; i++) fill(dist[i],dist[i]+m,-1);

    dist[0][0] = 0;
    Q.push(pair{0,0});

    while(!Q.empty()){
        pair<int,int> cur = Q.front();
        Q.pop();

        for(int dir = 0; dir < 4; dir++){
            int nx = dx[dir] + cur.first;
            int ny = dy[dir] + cur.second;

            if(nx < 0 || nx >= n || ny < 0  || ny >= m) continue;
            if(board[nx][ny] != '1' || dist[nx][ny] >= 0) continue;
            dist[nx][ny] = dist[cur.first][cur.second]+1;

            Q.push(pair{nx,ny});
        }
    }

    cout << dist[n-1][m-1] + 1;
}