#include <bits/stdc++.h>
using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};
vector<vector<char>> board;
vector<vector<char>> newboard;
vector<vector<char>> newboard2;
int n , m = 0;

void func(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            newboard[i][j] = 'O';
            
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(board[i][j] == 'O'){
                int nx = 0;
                int ny = 0;
                newboard[i][j] = '.';
                for(int k = 0; k < 4; k++){
                    nx = j + dx[k];
                    ny = i + dy[k];
                    if (nx < 0 || nx >= m) continue;
                    if (ny < 0 || ny >= n) continue;
                    newboard[ny][nx] = '.';
                }
            }
            
        }
    }
}

void func2(){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            newboard2[i][j] = 'O';
            
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(newboard[i][j] == 'O'){
                int nx = 0;
                int ny = 0;
                newboard2[i][j] = '.';
                for(int k = 0; k < 4; k++){
                    nx = j + dx[k];
                    ny = i + dy[k];
                    if (nx < 0 || nx >= m) continue;
                    if (ny < 0 || ny >= n) continue;
                    newboard2[ny][nx] = '.';
                }
            }
            
        }
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    
    int t = 0;

    cin >> n >> m >> t;
    
    board.resize(n, vector<char> (m));
    newboard.resize(n, vector<char> (m));
    newboard2.resize(n, vector<char> (m));

    for(int i = 0; i < n; i++){
        string s = "";
        cin >> s;
        for(int j = 0; j < s.size(); j++){
            board[i][j] = s[j];
        }
    }

    func();
    func2();
    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < m; j++){
    //         cout << newboard[i][j];
    //     }
    //     cout << '\n';
    // }
    if(t == 1){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                cout << board[i][j];
            }
            cout << '\n';
        }
    }else if(t % 4 == 2){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                cout << 'O';
            }
            cout << '\n';
        }
    }else if(t % 4 == 3){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                cout << newboard[i][j];
            }
            cout << '\n';
        }
    }else if(t % 4 == 0){
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                cout << 'O';
            }
            cout << '\n';
        }
    }else{
        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                cout << newboard2[i][j];
            }
            cout << '\n';
        }
    }

    return 0;
}