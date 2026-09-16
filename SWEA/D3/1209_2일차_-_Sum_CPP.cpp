#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    for(int t = 0; t < 10; t++){
        int tc = 0;
        cin >> tc;

        int mx = 0;
        int sum = 0;

        vector<vector<int>> board;
        board.assign(100, vector<int> (100));
        
        for(int i = 0; i < 100; i++){
            for(int j = 0; j < 100; j++){
                cin >> board[i][j];
            }
        }

        for(int i = 0; i < 100; i++){
            sum = 0;
            for(int j = 0; j < 100; j++){
                sum += board[i][j];
            }
            mx = max(sum, mx);
        }

        for(int j = 0; j < 100; j++){
            sum = 0;
            for(int i = 0; i < 100; i++){
                sum += board[i][j];
            }
            mx = max(sum, mx);
        }
        
        sum  = 0;

        for(int i = 0; i < 100; i++){
            sum += board[i][i];
        }

        mx = max(sum, mx);

        sum = 0;
        int a = 99;
        for(int i = 0; i < 100; i++){
            sum += board[a-i][a-i];
        }

        mx = max(sum, mx);
        cout << '#' << tc << ' ' << mx << '\n';
    }

    

    return 0;
}