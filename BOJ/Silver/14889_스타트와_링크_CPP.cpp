#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> board;
vector<int> vis;

int n = 0;
int mn = 1e9;

int calc(){
    int sum1 = 0;
    int sum2 = 0;
    int res = 0;
    vector<int> t1;
    vector<int> t2;

    for(int i = 0; i < n; i++){
        if(vis[i] == true){
            t1.push_back(i);
        }else{
            t2.push_back(i);
        }
    }

    for(int i = 0; i < n/2; i++){
        for(int j = i+1; j < n/2; j++){
            sum1 += (board[t1[i]][t1[j]] + board[t1[j]][t1[i]]);
            sum2 += (board[t2[i]][t2[j]] + board[t2[j]][t2[i]]);
        }
    }

    if(sum1 > sum2){
        res = sum1 - sum2;
    }else{
        res = sum2 - sum1;
    }
    return res;
}

void dfs(int k, int idx){
    if(k == n/2){
        int cur = calc();
        mn = min(cur, mn);
        return;
    }

    for(int i = idx; i < n; i++){
        if(!vis[i]){
            vis[i] = true;
            dfs(k+1, i+1);
            vis[i] = false;
        }
    }

}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;

    board.resize(n, vector<int> (n));
    vis.resize(n);
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> board[i][j];
        }
    }

    dfs(0, 0);
    cout << mn;

    return 0;
}