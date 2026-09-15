#include <stack>
#include <vector>
#include <iostream>

using namespace std;

int solution(vector<int> order) {
    int answer = 0;  
    
    stack<int> st;
    
    int cnt = 1;
    
    for(int i = 0; i < order.size(); i++){
        while(cnt <= order[i]){
            st.push(cnt);
            cnt++;
        }
        
        if(order[i] == st.top()){
            st.pop();
            answer++;
        }else{
            break;
        }
    }

    return answer;  // 결과 반환
}
