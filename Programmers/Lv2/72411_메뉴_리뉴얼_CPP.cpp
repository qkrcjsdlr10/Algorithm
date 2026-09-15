#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <algorithm>

using namespace std;

map<string, int> m;

void dfs(string &order, int sz, string str, int idx, int target){
    if(sz == target){
        cout << str << ' ';
        m[str]++;
        return;
    }
    
    for(int i = idx; i < order.size(); i++){
        str.push_back(order[i]);
        dfs(order, sz+1, str, i + 1, target);
        str.pop_back();
    }
}


vector<string> solution(vector<string> orders, vector<int> course) {
    vector<string> answer;
    
    vector<int> arr(26);
    
    for(int i = 0; i < orders.size(); i++){
        sort(orders[i].begin(), orders[i].end());
        
        for(auto len : course){
            dfs(orders[i], 0, "", 0, len);
        }
    }
    
    for (int len : course) {
        int mx = 0;

        // 1) len 길이 조합 중 최대 빈도 찾기 (2 이상만)
        for (auto &p : m) {
            if ((int)p.first.size() == len && p.second >= 2)
                mx = max(mx, p.second);
        }

        // 2) 그 최대 빈도인 조합들 answer에 추가
        for (auto &p : m) {
            if ((int)p.first.size() == len && p.second == mx && mx >= 2)
                answer.push_back(p.first);
        }
    }

    sort(answer.begin(), answer.end());
    
    return answer;
}