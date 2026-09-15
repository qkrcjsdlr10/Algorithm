#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int res = 0;
set<vector<string>> st;
vector<bool> vis;
vector<string> temp;

int match(string s1, string s2){
    int temp = 0;
    if(s1.size() != s2.size()) return 0;
    
    for(int i = 0; i < s1.size(); i++){
        if(s2[i] == '*' || s1[i] == '*'){
            temp++;
            continue;
        }
        if(s1[i] == s2[i]) temp++;
        else return 0;
    }
    return 1;
}

void dfs(vector<string> user_id, vector<string> banned_id, int depth){
    if(depth == banned_id.size()){
        vector<string> t = temp;
        sort(t.begin(), t.end());
        st.insert(t);
        return;
    }
    
    for(int i = 0; i < user_id.size(); i++){
        if(vis[i]) continue;
        if(!match(user_id[i], banned_id[depth])) continue;
        
        vis[i] = true;
        temp.push_back(user_id[i]);
        dfs(user_id, banned_id, depth + 1);
        vis[i] = false;
        temp.pop_back();
    }
}

int solution(vector<string> user_id, vector<string> banned_id) {
    int answer = 0;
    
    vis.resize(user_id.size());
    
    dfs(user_id, banned_id, 0);
    
    for(auto x : st){
        for(auto s : x){
            cout << s << ' ';
        }
        cout << '\n';
    }
    
    answer = st.size();
    
    return answer;
}