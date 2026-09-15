#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

long long solution(int n, vector<int> works) {
    long long answer = 0;
    
    priority_queue<int> q;
    
    for(auto a : works){
        q.push(a);
    }
    
    for(int i = 0; i < n; i++){
        int cur = q.top(); q.pop();
        // cout << cur << ' ';
        if(cur > 0){
            cur--;
            q.push(cur);
        }else{
            break;
        }
    }
    
    while(!q.empty()){
        
        int cur = q.top(); q.pop();
        // cout << cur << ' ';
        answer += cur * cur;
    }
    
    return answer;
}