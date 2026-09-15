#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(vector<string> survey, vector<int> choices) {
    string answer = "";
    
    string s = "RTCFJMAN";
    vector<int> arr(8);
    
    int n = survey.size();
    
    for(int i = 0; i < n; i++){
        int score = abs(4 - choices[i]);
        
        int idx = 0;
        if(choices[i] == 4) continue;
        
        if(choices[i] > 4){
            idx = s.find(survey[i][1]);
            arr[idx] += score;
        }else{
            idx = s.find(survey[i][0]);
            arr[idx] += score;
        }
    }
    
    for(int i = 0; i < s.size(); i +=2){
        if(arr[i] < arr[i+1]){
            answer.push_back(s[i+1]);
        }else{
            if(arr[i] == arr[i+1]){
                if(s[i] < s[i+1]){
                    answer.push_back(s[i]);
                }else{
                    answer.push_back(s[i+1]);
                }
            }else{
                answer.push_back(s[i]);
            }
            
        }
        // cout << i << ' ' << i+1 << '\n';
    }
    
    for(auto c : arr){
        cout << c;
    }
    
    return answer;
}