#include <string>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

set<int> res;
vector<bool> vis;

void dfs(string& numbers, string num){
    // cout << "a";
    if(num[0] == '0') return;
    // cout << num << ' ' << num.size() << '\n';
    if(num.size() != 0){
        res.insert(stoi(num));
    } 
    
    for(int i = 0; i < numbers.size(); i++){
        // cout << numbers[i];
        if(vis[i]) continue;
        vis[i] = true;
        dfs(numbers, num + numbers[i]);
        vis[i] = false;
    }
    
}


int solution(string numbers) {
    int answer = 0;
    
    vis.resize(numbers.size());
    
    dfs(numbers, "");
    
    cout << '\n';
    
    for(auto a : res){
        // cout << a << ' ';
        if(a == 1) continue;
        int n = 2;
        bool check = false;
        while(n*n <= a){
            if(a % n == 0){
                check = true;
                // cout << a << ' ';
                break;
            } 
            n++;
        }
        if(!check){
            
            cout << a << ' ';
            answer++;
        }
    }
    
    return answer;
}