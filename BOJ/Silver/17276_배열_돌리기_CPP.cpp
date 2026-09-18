#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> arr;
vector<vector<int>> board;

void func(vector<vector<int>>& arr, int cnt, int dir){
    board = arr;
    if(cnt == 0) return;
    int n = arr.size();
    board = arr;
    if(dir == 1){
        for(int i = 0; i < n; i++){
            board[i][n/2] = arr[i][i];
            board[i][n-1-i] = arr[i][n/2];
            board[n/2][n-1-i] = arr[i][n-1-i];
            board[i][i] = arr[n/2][i];
        }
    }else{
        for(int i = 0; i < n; i++){
            board[n/2][i] = arr[i][i];
            board[n-1-i][i] = arr[n/2][i];
            board[n-1-i][n/2] = arr[n-1-i][i];
            board[n-1-i][n-1-i] = arr[n-1-i][n/2];
        }
    }
    arr = board;
    func(arr, cnt - 1, dir);
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int angle = 0;
    int cnt = 0;
    int tc = 0;
    cin >> tc;

    for(int t = 0; t < tc; t++){
        cin >> n >> angle;

        int dir = (angle >= 0 ? 1 : -1); 
        cnt = (abs(angle) / 45) % 8;
        arr.assign(n, vector<int>(n));
        board.assign(n, vector<int>(n));
        
        arr.resize(n, vector<int> (n));
        board.resize(n, vector<int> (n));

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> arr[i][j];
            }
        }

        func(arr, cnt, dir);

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cout << board[i][j] << ' ';
            }
            cout << '\n';
        }
    }

    return 0;
}