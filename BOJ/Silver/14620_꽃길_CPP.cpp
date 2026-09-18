#include <bits/stdc++.h>
using namespace std;

int nx[4] = {0, 1, 0, -1};
int ny[4] = {-1, 0, 1, 0};

vector<vector<int>> arr;
vector<vector<int>> vis;
int n = 0;
int res = 1e9;

int func(int y, int x){
    int sum = 0;
    for(int i = 0; i < 4; i++){
        sum += arr[y+ny[i]][x+nx[i]];
    }
    sum += arr[y][x];

    return sum;
}

void func1(int y, int x){
    for(int i = 0; i < 4; i++){
        vis[y+ny[i]][x+nx[i]] = 1;
    }
    vis[y][x] = 1;
}

void func2(int y, int x){
    for(int i = 0; i < 4; i++){
        vis[y+ny[i]][x+nx[i]] = 0;
    }
    vis[y][x] = 0;
}

bool canFlower(int y, int x){
    if(vis[y][x] == 1) return false;
    for(int i = 0; i < 4; i++){
        if(vis[y+ny[i]][x+nx[i]] == 1) return false;
    }
    return true;
}

void dfs(int cnt, int cost){
    if(cnt == 3){
        res = min(cost, res);
        return;
    }

    for(int i = 1; i < n-1; i++){
        for(int j = 1; j < n-1; j++){
            if(!canFlower(i, j)) continue;
            func1(i, j);
            dfs(cnt+1, cost + func(i, j));
            func2(i, j);
        }
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n;
    arr.resize(n, vector<int> (n));
    vis.resize(n, vector<int> (n));

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cin >> arr[i][j];
        }
    }

    dfs(0, 0);

    cout << res;

    return 0;
}   