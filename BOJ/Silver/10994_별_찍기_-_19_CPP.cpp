#include <bits/stdc++.h>
using namespace std;

vector<vector<char>> board;


void draw(int n, int start){
    if(n == 0) return;

    int len = 4*n - 3;
    int end = start + len - 1;

    for(int i = start; i <= end; i++){
        board[start][i] = '*';
        // cout << "asdfasdf";
    }

    for(int i = start; i <= end; i++){
        board[i][start] = '*';
        // cout << "asdfasdf";
    }

    for(int i = start; i <= end; i++){
        board[i][end] = '*';
        // cout << "asdfasdf";
    }

    for(int i = start; i <= end; i++){
        board[end][i] = '*';
        // cout << "asdfasdf";
    }

    draw(n - 1, start + 2);
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;

    cin >> n;

    board.resize(4*n-3, vector<char> (4*n-3, ' '));

    int sz = 4*n-3;

    draw(n, 0);

    for(int i = 0; i < sz; i++){
        for(int j = 0; j < sz; j++){
            cout << board[i][j];
        }
        cout << '\n';
    }

    return 0;
}