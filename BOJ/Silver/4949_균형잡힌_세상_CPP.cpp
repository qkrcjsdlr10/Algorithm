#include <bits/stdc++.h>
using namespace std;

int main(){
    string str;
    stack<char> s;
    vector<string> result;

    while(getline(cin, str) && str != "."){
        s = stack<char>(); // 새 문자열 처리 전 스택 초기화
        bool balanced = true;

        for(auto c : str){
            if(c == '(' || c == '['){
                s.push(c);
            } else if(c == ')' || c == ']'){
                if(s.empty() || 
                   (c == ')' && s.top() != '(') || 
                   (c == ']' && s.top() != '[')){
                    balanced = false;
                    break;
                }
                s.pop();
            }
        }

        result.push_back(balanced && s.empty() ? "yes" : "no");
    }

    for(const auto& r : result){
        cout << r << '\n';
    }

    return 0;
}