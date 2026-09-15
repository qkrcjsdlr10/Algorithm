#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(string begin, string target, vector<string> words) {
    int answer = 0;
    int res = 0;
    bool check = false;
    
    queue<pair<string, int>> q;
    vector<bool> vis(words.size(), 0);
    
    q.push({begin, 0});
    
    while(!q.empty()){
        auto cur = q.front(); q.pop();
        string word = cur.first;
        int cnt = cur.second;
        
        for(int i = 0; i < words.size(); i++){
            if(vis[i]) continue;
            
            int diff = 0;
            for(int j = 0; j < word.size(); j++){
                if(word[j] != words[i][j]){
                    diff++;
                }
            }
            if(diff == 1){
                if(words[i] == target){
                    return cnt + 1;
                }
                vis[i] = true;
                q.push({words[i], cnt+1});
            }
        }
    }
    
    return answer;
}