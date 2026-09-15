#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <iostream>

using namespace std;

int solution(string s) {
    int answer = 0;
    
    stack<char> st;
    
    deque<char> qq;
    
    for(int i = 0; i < s.size(); i++){
        qq.push_back(s[i]);
    }
    
    for(int i = 0; i < s.size(); i++){
        stack<char> st;
        deque<char> q;
        q = qq;
        // for(int j = 0; j < qq.size(); j++){
        //     cout << q[j];
        // }
        // cout << '\n';
        while(!q.empty()){
            if(st.empty()){
                st.push(q.front());
                q.pop_front();
                continue;
            }
            if(!st.empty() && st.top() == '[' && q.front() == ']'){
                st.pop();
            }else if(!st.empty() && st.top() == '{' && q.front() == '}'){
                st.pop();
            }else if(!st.empty() && st.top() == '(' && q.front() == ')'){
                st.pop();
            }else{
                st.push(q.front());
            }
            
            q.pop_front();
        }
        if(st.empty()){
            answer++;
        }
        qq.push_back(qq.front());
        qq.pop_front();
        
    }
    
    return answer;
}