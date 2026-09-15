#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(vector<int> food) {
    string answer = "";
    
    for(int i = 1; i < food.size(); i++){
        int a = food[i] / 2;
        if(a > 0){
            for(int j = 0; j < a; j++){
                answer += i + '0';
            }
        }
    }
    
    answer += '0';
    
    for(int i = answer.size()-2; i >= 0 ; i--){
        answer += answer[i];
    }
    
    return answer;
}