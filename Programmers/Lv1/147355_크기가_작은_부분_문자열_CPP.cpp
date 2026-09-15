#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(string t, string p) {
    int answer = 0;
    
    for(int i = 0; i <= t.size() - p.size(); i++){
        string str = "";
        for(int j = i; j < p.size() + i; j++){
            str += t[j];
        }
        // int a = stoi(str);
        // cout << a << ' ';
        if(str <= p){
            answer++;
        }
    }
    
    return answer;
}