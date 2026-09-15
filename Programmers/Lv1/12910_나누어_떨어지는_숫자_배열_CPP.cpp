#include <string>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> solution(vector<int> arr, int divisor) {
    vector<int> answer;
    int temp = 0;
    
    for(int i = 0; i < arr.size(); i++){
        if(arr[i] >= divisor && arr[i] % divisor == 0){
            answer.push_back(arr[i]);
        }
    }
    
    
    for(int i = 0; i < answer.size(); i++){
        for(int j = i+1; j < answer.size(); j++){
            if(answer[i] > answer[j]){
                temp = answer[j];
                answer[j] = answer[i];
                answer[i] = temp;
            }
        }
    }
    
    // sort(answer.begin(), answer.end());
    
    if(answer.size() == 0){
        answer.push_back(-1);
        
    }
    
    return answer;
}