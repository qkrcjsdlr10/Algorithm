#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(vector<string> gems) {
    vector<int> answer;
    
    unordered_map<string , int> m;
    set<string> st;
    int left = 0;
    int right = 0;
    int mn = 1e9;
    int ansL = 0, ansR = 0;
    
    for (const string& g : gems) {
        st.insert(g);
    }
    
    while(right < gems.size()){
        m[gems[right]]++;
        while(m.size() == st.size()){
            if (mn > right - left + 1) {
                mn = right - left + 1;
                ansL = left;
                ansR = right;
            }
            
            m[gems[left]]--;
            if (m[gems[left]] == 0) {
                m.erase(gems[left]);
            }
            left++;
        }
        right++;
    }
    cout << left << ' ' << right;
    return {ansL + 1, ansR + 1};
}