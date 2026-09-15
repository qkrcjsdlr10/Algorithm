#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<int> solution(vector<int> answers) {
    vector<int> answer;
    vector<int> num1 = {1, 2, 3, 4, 5};
    vector<int> num2 = {2, 1, 2, 3, 2, 4, 2, 5};
    vector<int> num3 = {3, 3, 1, 1, 2, 2, 4, 4, 5, 5};
    int sum1 = 0, sum2 = 0, sum3 = 0;
    cout << answers.size() <<'\n';
    for(int i = 0; i < answers.size(); i++){
        if(num1[i % num1.size()] == answers[i]){
            sum1++;
        }
        if(num2[i % num2.size()] == answers[i]){
            sum2++;
        }
        if(num3[i % num3.size()] == answers[i]){
            sum3++;
        }
    }
    
    int mx = max(sum1, sum2);
    mx = max(mx, sum3);
    
    cout << sum1 << ' ' << sum2 << ' ' << sum3 << ' ' << mx;
    
    if(mx == sum1){
        answer.push_back(1);
    }
    if(mx == sum2){
        answer.push_back(2);
    }
    if(mx == sum3){
        answer.push_back(3);
    }
    
    return answer;
}