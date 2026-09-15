#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> res;

char arr[5] = {'A', 'E', 'I', 'O', 'U'};

void dfs(string s) {
    if (s.size() == 5) return;

    for (int i = 0; i < 5; i++) {
        string next = s + arr[i];
        res.push_back(next);
        dfs(next);
    }
}

int solution(string word) {
    int answer = 0;
    string str = "";
    dfs(str);
    
    for(auto s : res){
        if(s == word){
            answer++;
            break;
        }
        answer++;
    }
    
    return answer;
}