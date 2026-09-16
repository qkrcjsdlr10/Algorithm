#include<bits/stdc++.h>
using namespace std;

int n = 0;
vector<vector<int>> board;
int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

bool samecheck(int y, int x, int c, int dir){
    int nx = x + dx[dir];
    int ny = y + dy[dir];
    if(nx >= n || nx < 0 || ny >= n || ny < 0) return 0;
    if(!board[ny][nx]) return 0;
    if(board[ny][nx] + c != 3) return 0;
    return 1;
}

void flip(int y, int x, int c, int dir){
    int canx = x;
    int cany = y;
    while(1){
        int nx = canx + dx[dir];
        int ny = cany + dy[dir];
        if(nx >= n || nx < 0 || ny >= n || ny < 0) return;
        if(!board[ny][nx]) return;

        canx = nx;
        cany = ny;
        if(board[cany][canx] == c){
            break;
        }
    }
    int nx = x + dx[dir];
    int ny = y + dy[dir];

    while(board[ny][nx] != c){
        board[ny][nx] = c;
        nx += dx[dir];
        ny += dy[dir];
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        n = 0;
        int m = 0;

        cin >> n >> m;
        vector<vector<int>> cmd(m, vector<int>(3));
        board.assign(n, vector<int>(n, 0));

        board[n/2][n/2] = 2;
        board[n/2-1][n/2-1] = 2;
        board[n/2][n/2-1] = 1;
        board[n/2-1][n/2] = 1;

        for(int i = 0; i < m; i++){
            int x = 0, y = 0, c = 0;
            cin >> x >> y >> c;
            x--;
            y--;
            cmd[i][0] = x;
            cmd[i][1] = y;
            cmd[i][2] = c;
        }

        for(int i = 0; i < m; i++){
            int x = cmd[i][1];
            int y = cmd[i][0];
            int c = cmd[i][2];

            board[y][x] = c;
            for(int j = 0; j < 8; j++){
                bool same = samecheck(y, x, c, j);
                if(same){
                    flip(y, x, c, j);
                }
            }
        }

        // for(int i = 0; i < 8; i++){
        //     int nx = n/2 + dx[i];
        //     int ny = n/2 + dy[i];
        //     cout << board[ny][nx] << ' ';
        // }
        // cout << '\n';
        int w = 0, b = 0;

        for(auto bo : board){
            for(auto bc : bo){
                if(bc == 2){
                    w++;
                }else if(bc == 1){
                    b++;
                }
                // cout << b << ' ';
            }
            // cout << '\n';
        }
        cout << '#' << t << ' ' << b << ' ' << w << '\n';
    }
    
    
    return 0;
}