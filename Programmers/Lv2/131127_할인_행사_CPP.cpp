#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

int solution(vector<string> want, vector<int> number, vector<string> discount) {
    int answer = 0;
    
    vector<int> check;
    
    check.resize(want.size());
    
    for(int i = 0; i <= discount.size()-10; i++){
        bool b = true;
        fill(check.begin(), check.end(), 0);
        for(int j = i; j < i+10; j++){
            // cout << j << ' ';
            for(int k = 0; k < want.size(); k++){
                if(discount[j] == want[k]){
                    check[k]++;
                }
            }
        }
        
        for(int k = 0; k < want.size(); k++){
            if(check[k] < number[k]){
                
                // cout << i << " asdf ";
                b = false;
            }
        }
        if(b == true){
            answer++;
        }
    }
    
    return answer;
}