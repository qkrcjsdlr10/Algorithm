#include<bits/stdc++.h>
using namespace std;

int arr[50][50];
int x, y, a, n, m, cnt;
int b, nx, ny, na;
int bx, by, ba;

// stack<pair<int, int>> s;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

void dfs();

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m >> y >> x >> a;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> arr[i][j];
        }
    }

    dfs();

    cout << cnt;
    return 0;
}

void dfs(){
    if(arr[y][x] == 0){
        cnt++;
        arr[y][x] = -1;
    }

    for(int i = 0; i < 4; i++){
        na = (a + 3 - i) % 4;
        nx = x + dx[na];
        ny = y + dy[na];

        if(nx < 0 || nx >= m || ny < 0 || ny >= n){
            continue;
        }
        if(arr[ny][nx] == 1 || arr[ny][nx] == -1){
            continue;
        }

        if(arr[ny][nx] == 0){
            x = nx;
            y = ny;
            a = na;

            cnt++;
            arr[y][x] = -1;
            dfs();
        }

    }

    ba = (a + 2) % 4;
    bx = x + dx[ba];
    by = y + dy[ba];

    if(bx < 0 || bx >= m || by < 0 || by >= n){
        return;
    }

    if(arr[by][bx] != 1){
        x = bx;
        y = by;
        dfs();
    }

    if(bx < 0 || bx >= m || by < 0 || by >= n){
        return;
    }

    if(arr[by][bx] == 1){
        return;
    }
}