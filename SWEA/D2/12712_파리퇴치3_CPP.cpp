#include <bits/stdc++.h>
using namespace std;

int stcolpos(int y, int x, int sz){
    if(x - sz + 1 >= 0){
        return x - sz + 1;
    }else{
        return 0;
    }
}

int edcolpos(int y, int x, int sz, int n){
    if(x + sz - 1 < n ){
        return x + sz - 1;
    }else{
        return n - 1;
    }
}

int strowpos(int y, int x, int sz){
    if(y - sz + 1 >= 0 ){
        return y - sz + 1;
    }else{
        return 0;
    }
}

int edrowpos(int y, int x, int sz, int n){
    if(y + sz - 1 < n ){
        return y + sz - 1;
    }else{
        return n - 1;
    }
}

int sumres(vector<vector<int>> &board, int y, int x, int sz, int n){
    int sum1 = 0;
    int sum2 = 0;
    int check1 = y + x;
    int check2 = y - x;

    int stcol = stcolpos(y, x, sz);
    int edcol = edcolpos(y, x, sz, n);
    int strow = strowpos(y, x, sz);
    int edrow = edrowpos(y, x, sz, n);

    for(int i = strow; i <= edrow; i++){
        for(int j = stcol; j <= edcol; j++){
            if(i + j == check1 || i - j == check2){
                sum1 += board[i][j];
            }

            if(i == y || j == x){
                sum2 += board[i][j];
            }
            // cout << board[i][j] << ' ';
        }
        // cout << '\n';
    }
    // cout << '\n';

    return max(sum1, sum2);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc=  0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        int n = 0, sz = 0;
        int res = 0;
        cin >> n >> sz;

        vector<vector<int>> board(n, vector<int>(n));

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> board[i][j];
            }
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                int sum = sumres(board, i, j, sz, n);
                res = max(res, sum);
            }
        }
        
        
        cout << '#' << t << ' ' << res << '\n';
    }

    
    

    return 0;
}