#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(string s) {
    vector<int> answer;
    answer.push_back(-1);
    for(int i = 1; i < s.size(); i++){
        bool a  = false;
        // cout << s[i];
        for(int j = i-1; j >= 0; j--){
            if(s[j] == s[i]){
                answer.push_back(i-j);
                a = true;
                break;
            }
            
        }
        if(a == false){
            answer.push_back(-1);
        }
    }
    
    return answer;
}