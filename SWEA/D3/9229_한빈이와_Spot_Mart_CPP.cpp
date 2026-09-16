#include <bits/stdc++.h>
using namespace std;

int n = 0, m = 0, mx = 0;
vector<bool> vis;
vector<int> arr;

void dfs(int depth, int sum, int cnt, int idx){
    // cout << sum << ' ';
    if(cnt >= 2){
        if(sum <= m){
            mx = max(sum, mx);
        }
        
        return;
    }

    for(int i = 0; i < n; i++){
        if(i < idx) continue;
        if(!vis[i]){
            vis[i] = true;
            dfs(depth + 1, sum + arr[i], cnt + 1, i);
            vis[i] = false;
        }
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc;
    cin >> tc;
    for(int t = 1; t <= tc; t++){
        mx = 0;
        cin >> n >> m;
        arr.assign(0, 0);
        vis.assign(n, 0);

        for(int i = 0; i < n; i++){
            int num = 0;
            cin >> num;
            arr.push_back(num);
        }

        
        dfs(0, 0, 0, 0);
        
        cout << '#' << t << ' ' << ((mx != 0) ? mx : -1) << '\n';
    }
    
    
    return 0;
}