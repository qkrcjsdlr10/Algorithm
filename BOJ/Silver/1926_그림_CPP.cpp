#include <bits/stdc++.h>
using namespace std;
#define X first
#define Y second


bool vis[502][502];
int dx[4] = {1,0,-1,0}, dy[4] = {0,1,0,-1};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int x = 0, y = 0;
    int num = 0;
    int mx = 0;
    int area = 0;
    queue<pair<int, int>> Q;

    cin >> x >> y;
    int arr[x][y] = {};

    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            cin >> arr[i][j];
        }
    }

    for(int i = 0; i < x; i++){
        for(int j = 0; j < y; j++){
            if(arr[i][j] == 0 || vis[i][j]) continue;
            num++;
            vis[i][j] = 1;
            Q.push({i,j});
            area = 0;
            while(!Q.empty()){
                area++;
                pair<int, int> cur = Q.front(); 
                Q.pop();
                
                
                for(int dir = 0; dir < 4; dir++){
                    int nx = cur.X + dx[dir];
                    int ny = cur.Y + dy[dir];

                    if(nx < 0 || nx >= x || ny < 0 || ny >= y) continue;
                    if(vis[nx][ny] || arr[nx][ny] != 1) continue;
                    vis[nx][ny] = 1;

                    Q.push({nx, ny});
                }
            }
            mx = max(mx, area);
        }
    }



    cout << num << '\n' << mx;
}