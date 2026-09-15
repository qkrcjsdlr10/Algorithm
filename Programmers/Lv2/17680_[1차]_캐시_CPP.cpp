#include <string>
#include <vector>
#include <deque>
#include <algorithm>
#include <cctype> 
#include <iostream>

using namespace std;

int solution(int cacheSize, vector<string> cities) {
    int answer = 0;
    if(cacheSize == 0){
        answer = cities.size() * 5;
        return answer;
    }
    deque<string> dq;
    
    for(int i = 0; i < cities.size(); i++){
        string str = cities[i];
        for (char &c : str) c = tolower((unsigned char)c);
        
        bool check = false;
        for(auto it = dq.begin(); it != dq.end(); it++){
            if(str == *it){
                check = true;
                dq.erase(it);
                break;
            }
        }
        
        if(check){
            answer++;
            dq.push_back(str);
        }else{
            if(dq.size() == cacheSize){
                dq.pop_front();    
            }
            dq.push_back(str);
            answer += 5;
        }
        // cout << answer << ' ';
    }
    
    return answer;
}