#include <bits/stdc++.h>
using namespace std;

vector<vector<char>> board;
int sz;
bool check = false;

void draw(int n, int start, bool check){
    if(n == 0) return;

    for(int i = start; i < sz-start; i++){
        if(check){
            if(i % 2 == 1){
                board[n-1][i] = '*';
            }
        }else{
            if(i % 2 == 0){
                board[n-1][i] = '*';
            }
        }
    }
    // cout << start << ' ';

    draw(n - 1, start + 1, !check);
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;

    cin >> n;

    sz = 2*n-1;

    board.resize(n, vector<char> (sz, ' '));

    draw(n, 0, false);
    // cout << sz;

    for(int i = 0; i < n; i++){
        int last = sz - 1;
        while(last >= 0 && board[i][last] == ' ') last--;

        for(int j = 0; j <= last; j++){
            cout << board[i][j];
        }
        cout << '\n';
    }

    return 0;
}