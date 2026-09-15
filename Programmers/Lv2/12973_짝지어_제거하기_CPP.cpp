#include <iostream>
#include<string>
#include <bits/stdc++.h>

using namespace std;

int solution(string s)
{
    int answer = -1;
    
    stack<char> st;
    
    // st.push(s[0]);
    for(int i = 0; i < s.size(); i++){
        if(!st.empty() &&st.top() == s[i]){
            st.pop();
        }else{
            st.push(s[i]);
        }
        
    }
    
    if(st.empty() == true){
        answer = 1;
    }else{
        answer = 0;
    }
    
    return answer;
}