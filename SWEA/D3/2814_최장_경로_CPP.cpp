#include<bits/stdc++.h>
using namespace std;

vector<vector<int>> graph;
vector<int> vis;

int mx = 1;
int n = 0;

void dfs(int cnt, int node){
    mx = max(mx, cnt);

    for(int next : graph[node]){
        if(vis[next]) continue;

        vis[next] = true;
        dfs(cnt + 1, next);
        vis[next] = false;
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        mx = 1;
        n = 0;
        graph.clear();
        vis.clear();

        int m = 0;
        cin >> n >> m;
        
        graph.resize(n + 1);
        vis.resize(n + 1);

        for(int i = 0; i < m; i++){
            int v = 0, u = 0;
            cin >> v >> u;
            graph[v].push_back(u);
            graph[u].push_back(v);
        }
        
        for(int i = 1; i <= n; i++){
            vis[i] = true;
            dfs(1, i);
            vis[i] = false;
        }

        cout << '#'<< t << ' ' << mx << '\n';
    }
    
    return 0;
}