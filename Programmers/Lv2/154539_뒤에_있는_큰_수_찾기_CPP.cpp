#include <string>
#include <vector>
#include <stack>

using namespace std;

vector<int> solution(vector<int> numbers) {
    vector<int> answer;
    
    stack<int> st;
    answer.assign(numbers.size(), -1);
    for(int i = 0; i < numbers.size(); i++){
        while(!st.empty() && numbers[st.top()] < numbers[i]){
            answer[st.top()] = numbers[i];
            st.pop();
        }
        st.push(i);
    }
    
    return answer;
}