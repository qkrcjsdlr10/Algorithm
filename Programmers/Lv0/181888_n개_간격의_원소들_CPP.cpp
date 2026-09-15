#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(vector<int> num_list, int n) {
    vector<int> answer;
    int cnt = 0;
    answer.push_back(num_list[0]);
    
    for(auto a : num_list){
        if(cnt == n){
            answer.push_back(a);
            cnt = 0;
            cnt++;
        }else{
            cnt++;
        }
        // cout << cnt;
    }
    
    return answer;
}