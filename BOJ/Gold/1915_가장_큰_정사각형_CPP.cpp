#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0, m = 0;

    cin >> n >> m;
    vector<vector<int>> board(n, vector<int> (m));

    for(int i = 0; i < n; i++){
            string s = "";
            cin >> s;
        for(int k = 0; k < s.size(); k++){
            board[i][k] = s[k] - '0';
        }
        
    }
    for(int i = 1; i < n; i++){
        for(int j = 1; j < m; j++){
            if(board[i][j] == 0) continue;
            board[i][j] = min({board[i-1][j], board[i][j-1], board[i-1][j-1]}) + 1;
        }
    }

    int mx = 0;
    for(auto bo : board){
        for(auto b : bo){
            if(mx < b){
                mx = b;
            }
            // cout << b << ' ';
        }
        // cout << '\n';
    }

    cout << mx * mx;

    return 0;
}