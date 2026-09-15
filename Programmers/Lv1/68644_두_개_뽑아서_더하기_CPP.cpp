#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(vector<int> numbers) {
    vector<int> answer;
    
    for(int i = 0; i < numbers.size()-1; i++){
        for(int j = i +1; j < numbers.size(); j++){
            bool check = false;
            int a = numbers[i] + numbers[j];
            for(int k = 0; k < answer.size(); k++){
                if(a == answer[k]){
                    check = true;
                    break;
                }
            }
            if(check == false){
                answer.push_back(a);
            }
        }
    }
    
    for(int i = 0; i < answer.size()-1; i++){
        for(int j = 0; j < answer.size() - 1 - i; j++){
            if(answer[j] > answer[j+1]){
                int temp = answer[j];
                answer[j] = answer[j+1];
                answer[j+1] = temp;
            }
        }
    }
    
    return answer;
}