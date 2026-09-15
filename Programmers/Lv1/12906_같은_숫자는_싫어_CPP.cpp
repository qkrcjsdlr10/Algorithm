#include <vector>
#include <stack>
#include <iostream>

using namespace std;

vector<int> solution(vector<int> arr) 
{
    vector<int> answer;

    stack<int> s;
    
    for(int i = 0; i < arr.size(); i++){
        if(s.empty() || s.top() != arr[i]){
            s.push(arr[i]);
        }
    }
    int idx = s.size() - 1;
    answer.resize(s.size());
    
    while(!s.empty()){
        answer[idx] = s.top(); s.pop();
        idx--;
    }

    return answer;
}