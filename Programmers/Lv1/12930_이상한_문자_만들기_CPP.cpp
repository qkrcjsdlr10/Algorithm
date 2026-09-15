#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string s) {
    string answer = "";
    int idx = 0;
    for(int i = 0; i < s.size(); i++){
        if(s[i] == ' '){
            idx = 0;
            answer += s[i];
            continue;  
        }
        
        if(idx % 2 == 0){
            if(s[i] >= 'a' && s[i] <= 'z') {
                s[i] = s[i] - 32;
            }
            answer += s[i];
            idx++;
        }else{
            if(s[i] >= 'A' && s[i] <= 'Z') {
                s[i] = s[i] + 32;
            }
            answer += s[i];
            idx++;
        }
    }
    // answer = s;
    cout << answer;
    
    return answer;
}