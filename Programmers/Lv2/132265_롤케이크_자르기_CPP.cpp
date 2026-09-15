#include <string>
#include <vector>
#include <set>
#include <iostream>

using namespace std;

int solution(vector<int> topping) {
    int answer = 0;
    int check[1000000] = {0};
    set<int> s1;
    set<int> s2;
    
    int cut = 1;
    
    for(int j = cut; j < topping.size(); j++){
        s2.insert(topping[j]);
        check[topping[j]]++;
        // cout << j;
    }
    int r = s2.size();
    
    for(int i = 0; i < topping.size(); i++){
        s1.insert(topping[i]);
        if(i == 0) continue;
        check[topping[i]]--;
        if(check[topping[i]] == 0){
            r--;
        }
        
        cut++;
        if(s1.size() == r){
            answer++;
        }
        // cout << s1.size() << ' ' << s2.size() << '\n';
        // s2.clear();
        // for(auto it : s1){
        //     cout << it;
        // }
        // cout << '\n';
        // for(auto it : s2){
        //     cout << it;
        // }
        // cout << '\n';
    }
    
    
    return answer;
}