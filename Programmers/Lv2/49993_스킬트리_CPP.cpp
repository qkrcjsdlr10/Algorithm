#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(string skill, vector<string> skill_trees) {
    int answer = 0;
    
    
    for(int i = 0; i < skill_trees.size(); i++){
        int cur  = 0;
        bool check = false;
        for(int j = 0; j < skill_trees[i].size(); j++){
            for(int k = 0; k < skill.size(); k++){
                if(skill[k] == skill_trees[i][j]){
                    if(k == cur){
                        cur++;
                    }else{
                        check = true;
                        break;
                    }
                }
            }
        }
        if(check == false){
            for(auto c : skill_trees[i]){
                cout << c;
            }
            cout << '\n';
            answer++;
        }
    }
    
    return answer;
}