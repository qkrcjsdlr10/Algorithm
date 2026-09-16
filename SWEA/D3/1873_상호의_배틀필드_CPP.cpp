#include <bits/stdc++.h>
using namespace std;

int dir = 0;

int dirSearch(char c){
    if(c == '^' || c == 'U'){
        dir = 0;
    }else if(c == 'v' || c == 'D'){
        dir = 1;
    }else if(c == '<' || c == 'L'){
        dir = 2;
    }else if(c == '>' || c == 'R'){
        dir = 3;
    }

    return dir;
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        int h = 0;
        int w = 0;
        int n = 0;
        int curx = 0;
        int cury = 0;
        int dir = 0;
        int nx[4] = {0, 0, -1, 1};
        int ny[4] = {-1, 1, 0, 0};
    
        vector<vector<char>> board;
        string move = "";
    
        cin >> h >> w;
        board.assign(h + 2, vector<char>(w + 2, ' '));
    
        for(int i = 1; i < h + 1; i++){
            string s = "";
            cin >> s;
            for(int j = 1; j < w + 1; j++){
                board[i][j] = s[j-1];
            }
        }
        
        cin >> n >> move;
    
        for(int i = 0; i < h + 2; i++){
            for(int j = 0; j < w + 2; j++){
                if(i == 0 || i == h + 1){
                    board[i][j] = '#';
                }
                if(j == 0 || j == w + 1){
                    board[i][j] = '#';
                }
                if(board[i][j] == '<' || board[i][j] == '>' || board[i][j] == '^' || board[i][j] == 'v'){
                    curx = j;
                    cury = i;
                }
            }
        }
    
        
        
        // cout << curx << ' ' << cury;
    
        dir = dirSearch(board[cury][curx]);
    
        for(int i = 0; i < n; i++){
            int x = curx;
            int y = cury;
            if(move[i] == 'S'){
                while(board[y][x] != '#'){
                    y = y + ny[dir];
                    x = x + nx[dir];
                    if(board[y][x] == '*'){
                        board[y][x] = '.';
                        break;
                    }
                }
            }else if(move[i] == 'U'){
                dir = 0;
                board[cury][curx] = '^';
                y = y + ny[dir];
                x = x + nx[dir];
                if(board[y][x] == '.'){
                    board[cury][curx] = '.';
                    board[y][x] = '^';
                    curx = x;
                    cury = y;
                }
            }else if(move[i] == 'D'){
                dir = 1;
                board[cury][curx] = 'v';
                y = y + ny[dir];
                x = x + nx[dir];
                if(board[y][x] == '.'){
                    board[cury][curx] = '.';
                    board[y][x] = 'v';
                    curx = x;
                    cury = y;
                }
            }else if(move[i] == 'L'){
                dir = 2;
                board[cury][curx] = '<';
                y = y + ny[dir];
                x = x + nx[dir];
                if(board[y][x] == '.'){
                    board[cury][curx] = '.';
                    board[y][x] = '<';
                    curx = x;
                    cury = y;
                }
            }else if(move[i] == 'R'){
                dir = 3;
                board[cury][curx] = '>';
                y = y + ny[dir];
                x = x + nx[dir];
                if(board[y][x] == '.'){
                    board[cury][curx] = '.';
                    board[y][x] = '>';
                    curx = x;
                    cury = y;
                }
                // cout << cury << ' ' << cury;
            }
    
            // for(int k = 0; k < h + 2; k++){
            //     for(int j = 0; j < w + 2; j++){
            //         cout << board[k][j] << ' ';
            //     }
            //     cout << '\n';
            // }
            // cout << '\n';
        }

        cout << '#' << t << ' ';
    
        for(int i = 1; i < h + 1; i++){
            for(int j = 1; j < w + 1; j++){
                cout << board[i][j];
            }
            cout << '\n';
        }
    }

    

    return 0;
}