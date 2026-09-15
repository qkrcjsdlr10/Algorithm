#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(int k, vector<int> score) {
    vector<int> answer;
    
    vector<int> arr;
    
    for(int i = 0; i < score.size(); i++){
        arr.push_back(score[i]);
        sort(arr.rbegin(), arr.rend());
        if(k > i){
            answer.push_back(arr[i]);
        }else{
            answer.push_back(arr[k-1]);
        }
    }
    
    return answer;
}