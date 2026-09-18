#include <bits/stdc++.h>
using namespace std;

vector<vector<char>> board;
int sz;

void draw(int n, int start){
    if(n == 0) return;
    

    board[n-1][start] = '*';
    board[n-1][sz-start-1] = '*';
    // cout << start << ' ';

    draw(n - 1, start + 1);
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;

    cin >> n;

    sz = 2*n-1;

    board.resize(n, vector<char> (sz, ' '));

    draw(n-1, 1);
    // cout << sz;

    for(int i = 0; i < sz; i++){
        board[n-1][i] = '*';
    }


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