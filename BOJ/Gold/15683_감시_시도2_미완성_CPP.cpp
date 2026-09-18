#include <bits/stdc++.h>
using namespace std;

int n,m;
int arr[100][100];
int vis[100][100];
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};
int x, y;
int nx, ny;
int detect;
int d, cam, cnt, cam, maxCnt;
bool c;
pair<int, int> cur;
queue<pair<int, int>> Q;
queue<int> C;

void func(int x, int y){
    // if(arr[x][y] == 0){
    //     cnt++;
    //     arr[x][y] = -1;
    // }
    cout << "x= " << x << "y= " << y << "\n";
    if(x < 0 || x >= m || y < 0 || y >= n) return;
    if (arr[x][y] == 6 || vis[x][y]) return;

    if(arr[y][x] == 1){
        for(int i = 0; i < 4; i++){
            // d = (d + 3 - i) % 4;
            nx = x + dx[i];
            ny = y + dy[i];

            if(nx < 0 || nx >= m || ny < 0 || ny >= n){
                if(cnt > maxCnt){
                    maxCnt = cnt;
                }else{
                    for(int i = 0; i < n; i++){
                        for(int j = 0; j < m; j++){
                            vis[i][j] = 0;
                        }
                    }
                }
                continue;
            }

            if(arr[nx][ny] != 6){
                if(arr[nx][ny] != 1 || arr[nx][ny] != 2 || arr[nx][ny] != 3 || arr[nx][ny] != 4 || 
                    arr[nx][ny] != 5){
                    vis[nx][ny] = -1;
                }
                cnt++;
                func(nx, ny);
            }

            // if(cnt > maxCnt){
            //     maxCnt = cnt;
            // }else{
            //     for(int i = 0; i < n; i++){
            //         for(int j = 0; j < m; j++){
            //             vis[i][j] = 0;
            //         }
            //     }
            // }
        }
    }
    Q.pop();
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> arr[i][j];
            if(arr[i][j] == 1){
                Q.push({i, j});
                C.push(arr[i][j]);
            }
        }
    }

    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < m; j++){
    //         cout << vis[i][j];
    //     }
    //     cout << '\n';
    // }
    cur = Q.front(); Q.pop();
    func(cur.first, cur.second);

    // cout << maxCnt;
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << vis[i][j] << ' ';
        }
        cout << '\n';
    }

    return 0;
}