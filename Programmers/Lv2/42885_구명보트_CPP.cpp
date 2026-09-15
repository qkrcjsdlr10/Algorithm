#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int solution(vector<int> people, int limit) {
    int answer = 0;
    
    sort(people.begin(), people.end());
    int cur = 0;
    
    for(int i = people.size()-1; i >= 0 ; i--){
        if(cur > i) break;
        
        if(people[i] + people[cur] <= limit){
            cout << people[i] + people[cur] << ' ';
            answer++;
            cur++;
        }else{
            answer++;
        }
        
        
    }
    
    return answer;
}