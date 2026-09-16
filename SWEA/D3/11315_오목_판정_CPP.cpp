#include <bits/stdc++.h>
using namespace std;

int n = 0;
vector<vector<char>> board;

int rowcheck(int y, int x){
    int res = 0;
    for(int i = x + 1; i < n; i++){
        // cout << board[y][i] << ' ';
        if(board[y][i] == 'o'){
            res++;
        }else{
            break;
        }
    }
    for(int i = x - 1; i >= 0; i--){
        if(board[y][i] == 'o'){
            res++;
        }else{
            break;
        }
    }

    return res;
}

int colcheck(int y, int x){
    int res = 0;
    for(int i = y + 1; i < n; i++){
        if(board[i][x] == 'o'){
            res++;
        }else{
            break;
        }
    }
    for(int i = y - 1; i >= 0; i--){
        if(board[i][x] == 'o'){
            res++;
        }else{
            break;
        }
    }

    return res;
}

int crosscheck1(int y, int x){
    int res = 0;
    //01 12 23

    for(int i = 1; i < n; i++){
        if(y + i >= n || x + i >= n) break;

        if(board[y + i][x + i] == 'o'){
            res++;
        }else{
            break;
        }
    }
    
    for(int i = 1; i < n; i++){
        if(y - i < 0 || x - i < 0) break;
        
        if(board[y - i][x - i] == 'o'){
            res++;
        }else{
            break;
        }
    }


    return res;
}

int crosscheck2(int y, int x){
    int res = 0;
    //01 12 23
    // cout << " asdf";
    for(int i = 1; i < n; i++){
        if(y - i < 0 || x + i >= n) break;
        // cout << board[y - i][x + i] << ' ';
        if(board[y - i][x + i] == 'o'){
            res++;
        }else{
            break;
        }
    }
    
    for(int i = 1; i < n; i++){
        if(y + i >= n || x - i < 0) break;
        // cout << board[y + i][x - i] << ' ';
        if(board[y + i][x - i] == 'o'){
            res++;
        }else{
            break;
        }
    }


    return res;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        bool ans = false;
    
        cin >> n;
        board.assign(n, vector<char>());

        for(int i = 0; i < n; i++){
            string s = "";
            cin >> s;
            for(int j = 0; j < s.size(); j++){
                board[i].push_back(s[j]);
            }
        }

        for(int i = 0; i < n; i++){
            if(ans) break;

            for(int j = 0; j < n; j++){
                if(board[i][j] == 'o'){
                    int a = 0;
                    a = rowcheck(i, j);
                    int b = 0;
                    b = colcheck(i, j);
                    int c = 0;
                    c = crosscheck1(i, j);
                    int d = 0;
                    d = crosscheck2(i, j);
                    // cout << d << '\n';
                    if(a >= 4 || b >= 4 || c >= 4 || d >= 4){
                        ans = true;
                        break;
                    }
                    // cout << a << ' ';
                }
            }
        }

        cout << '#' << t << ' ' << (ans ? "YES" : "NO") << '\n';
    }
    
    return 0;
}