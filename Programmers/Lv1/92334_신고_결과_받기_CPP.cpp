#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(vector<string> id_list, vector<string> report, int k) {
    vector<int> answer(id_list.size());
    
    set<pair<string, string>> st;
    unordered_map<string, int> m;
    
    for(auto id : report){
        string name1 = "";
        string name2 = "";
        
        stringstream ss(id);
        
        getline(ss, name1, ' ');
        getline(ss, name2);
        // cout << name1 << ' ' << name2 << '\n';
        
        st.insert({name1, name2});
    }
    
    // for(auto it : st){
    //     cout << it.first << ' ' << it.second << '\n';
    // }
    
    for(auto it : st){
        string n = it.second;
        m[n]++;
    }
    
    for(auto it : st){
        string name1 = it.first;
        string name2 = it.second;
        
        if(m[name2] >= k){
            for(int i = 0; i < id_list.size(); i++){
                if(name1 == id_list[i]){
                    answer[i]++;
                }
            }
        }
    }
    
    return answer;
}