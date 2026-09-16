#include <bits/stdc++.h>
using namespace std;


int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        int n = 0;
        cin >> n;
        int startx = 0;
        int starty = 0;
        int endx = 0;
        int endy = 0;
        
        queue<tuple<int, int, int>> q;
        vector<vector<int>> board;
        vector<vector<int>> temp;
        board.assign(n, vector<int>(n, 0));
        temp.assign(n, vector<int>(n, 1e9));

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> board[i][j];
            }
        }

        cin >> starty >> startx >> endy >> endx;
        temp[starty][startx] = 0;
        q.push({starty, startx, 0});


        while(!q.empty()){
            auto [cury, curx, curt] = q.front(); q.pop();

            for(int d = 0; d < 4; d++){
                int nx = curx + dx[d];
                int ny = cury + dy[d];
                int nt = curt + 1;

                if(nx >= n || nx < 0 || ny >= n || ny < 0) continue;
                if(board[ny][nx] == 1) continue;
                
                if(board[ny][nx] == 2 && curt % 3 != 2){
                    q.push({cury, curx, nt});
                    continue;
                    
                }
                if(temp[ny][nx] <= nt) continue;
                temp[ny][nx] = nt;
                q.push({ny, nx, nt});
            }
        }

        // cout << '\n';

        // for(int i = 0; i < n; i++){
        //     for(int j = 0; j < n; j++){
        //         cout << temp[i][j] << ' ';
        //     }
        //     cout << '\n';
        // }

        cout << '#' << t << ' ' << ((temp[endy][endx] != 1e9) ? temp[endy][endx] : -1) << '\n';
    }

    

    return 0;
}