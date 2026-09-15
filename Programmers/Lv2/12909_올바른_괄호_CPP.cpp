#include<string>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

bool solution(string s)
{
    bool answer = true;

    stack<char> st;
    
    if(s[0] == ')'){
        answer = false;
        return answer;
    }else{
        st.push(s[0]);
    }
    
    for(int i = 1; i < s.size(); i++){
        if(s[i] == ')' && st.empty() == false){
            if(st.top() == '(')
                st.pop();
            continue;
        }
        st.push(s[i]);    
        
    }
    
    if(st.empty() == false){
        answer = false;
    }

    return answer;
}