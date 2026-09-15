#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(int n, vector<vector<int>> edge) {
    int answer = 0;
    
    vector<vector<int>> graph(n + 1);
    queue<int> q;
    vector<int> dist(n + 1);
    dist[1] = 1;
    for (auto &e : edge) {
        int u = e[0];
        int v = e[1];

        graph[u].push_back(v);
        graph[v].push_back(u); 
    }
    q.push(1);
    
    while(!q.empty()){
        int cur = q.front(); q.pop();
        int d = dist[cur];
        
        for(int i = 0; i < graph[cur].size(); i++){
            int next = graph[cur][i];

            if(dist[next] == 0) { // 방문 안 했을 때만
                q.push(next);
                dist[next] = d + 1;
            }
        }
    }
    
    int mx = *max_element(dist.begin(), dist.end());
    
    for(auto a : dist){
        if(mx == a) answer++;
    }
    
    
    return answer;
}