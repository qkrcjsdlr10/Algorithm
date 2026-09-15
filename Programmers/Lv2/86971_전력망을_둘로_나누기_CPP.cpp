#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(int n, vector<vector<int>> wires) {
    int answer = -1;
    
    vector<vector<int>> graph(n+1);
    vector<bool> vis(n+1);
    
    int mn = 1e9;
    
    for(auto &w : wires){
        int a = w[0], b = w[1];
        graph[w[0]].push_back(w[1]);
        graph[w[1]].push_back(w[0]);
    }
    
    for(auto wi : wires){
        queue<int> q;
        q.push(1);
        vis.assign(n+1, 0);
        vis[1] = true;
        int cnt1 = 0;
        int a = wi[0], b = wi[1];
        
        
        
        while(!q.empty()){
            cnt1++;
            int cur = q.front();
            cout << cur << ' ';
            q.pop();
            for(auto w : graph[cur]){
                if(vis[w]) continue;
                if ((cur == a && w == b) || (cur == b && w == a))
                    continue;
                q.push(w);
                vis[w] = true;
            }
            // cout << cnt1 << ' ' << cnt2 << '\n';
        }
        
        int cnt2 = n - cnt1;
        mn = min(mn, abs(cnt1 - cnt2));
    }
    
    
    
    answer = mn;
    
    return answer;
}