#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string X, string Y) {
    string answer = "";
    
    vector<int> arr1(10);
    vector<int> arr2(10);
    vector<int> res(10);
    
    for(auto c : X){
        arr1[c - '0']++;
        // cout << c;
    }
    
    for(auto c : Y){
        arr2[c - '0']++;
    }
    
    for(int i = 9; i >= 0; i--){
        int mn = 0;
        if(arr1[i] != 0 && arr2[i] != 0){
            mn = min(arr1[i], arr2[i]);
        }
        res[i] = mn;
    }
    
    for(int i = 9; i >= 0; i--){
        for(int j = 0; j < res[i]; j++){
            if(answer.size() > 0){
                if(answer[0] != '0'){
                    answer += i + '0';
                }
            }else{
                if(answer[0] != '0'){
                    answer += i + '0';
                }
            }
        }
    }
    
    answer = (answer.size() == 0) ? "-1" : answer;
    
    return answer;
}