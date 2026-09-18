#include <bits/stdc++.h>

using namespace std;
char arr[8][8] = {};
int cnt1 = 0, cnt2 = 0;
int mn = 100;
int res = 100;
char tempBoard1[8][8] = {'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'
    ,'B', 'W', 'B', 'W', 'B', 'W', 'B','W'
    ,'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'
    ,'B', 'W', 'B', 'W', 'B', 'W', 'B','W'
    ,'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'
    ,'B', 'W', 'B', 'W', 'B', 'W', 'B','W'
    ,'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'
    ,'B', 'W', 'B', 'W', 'B', 'W', 'B','W'};

char tempBoard2[8][8] = {'B', 'W', 'B', 'W', 'B', 'W', 'B','W'
    ,'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'
    ,'B', 'W', 'B', 'W', 'B', 'W', 'B','W'
    ,'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'
    ,'B', 'W', 'B', 'W', 'B', 'W', 'B','W'
    ,'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'
    ,'B', 'W', 'B', 'W', 'B', 'W', 'B','W'
    ,'W', 'B', 'W', 'B', 'W', 'B', 'W', 'B'};

void func(){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(arr[i][j] != tempBoard1[i][j]){
                cnt1++;
            }
            if(arr[i][j] != tempBoard2[i][j]){
                cnt2++;
            }
        }
    }
    int temp = min(cnt1, cnt2);
    res = min(temp, res);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            // cout << tempBoard1[i][j];
        }
        // cout << '\n';
    }
    
    int n = 0, m = 0;

    cin >> n >> m;

    vector<vector<char>> board;
    board.resize(n, vector<char> (m));


    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> board[i][j];
        }
    }
    
    for(int i = 0; i <= n-8; i++){
        for(int j = 0; j <= m-8; j++){
            int x = 0, y = 0;
            for(int k = i; k < i+8; k++){
                for(int l = j; l < j+8; l++){
                    arr[y][x] = board[k][l];
                    // cout << arr[y][x];
                    x++;
                }
                x = 0;
                // cout << '\n';
                y++;
            }
            // cout << '\n';
            cnt1 = 0;
            cnt2 = 0;
            func();
            // for(int a = 0; a < 8; a++){
            //     for(int b = 0; b < 8; b++){
            //         cout << arr[a][b];
            //     }
            //     cout << '\n';
            // }
            // // cout << '\n';
        }
    }


    cout << res;
    return 0;
}