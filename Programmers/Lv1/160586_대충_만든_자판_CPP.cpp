#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

vector<int> solution(vector<string> keymap, vector<string> targets) {
    vector<int> answer;
    
    for(int i = 0; i < targets.size(); i++){
        string target = targets[i];
        int cnt = 0;
        size_t mn = string::npos;
        for(int j = 0; j < target.size(); j++){
            mn = string::npos;
            char t = target[j];
            for(int k = 0; k < keymap.size(); k++){
                if(keymap[k].find(t) != string::npos){
                    mn = min(mn, keymap[k].find(t));
                }
            }
            if (mn == string::npos) { // 이 문자 못 찾음
                cnt = -1;
                break;
            }
            cnt += mn + 1;
        }
        answer.push_back(cnt);
    }
    
    return answer;
}