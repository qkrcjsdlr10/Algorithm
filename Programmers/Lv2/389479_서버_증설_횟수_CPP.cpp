#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(vector<int> players, int m, int k) {
    int answer = 0;
    
    int sz = players.size();
    int cur = 0;
    vector<int> time(sz + k + 1);
    // cout << sz;
    
    for(auto a : players){
        cout << a << ' ';
    }
    
    for(int i = 0; i < sz; i++){
        // cout << players[i] << ' ';
        if(time[i] > 0){
            cur -= time[i];
        }
        if(players[i] >= m){
            
            int required = players[i] / m;
            if (cur < required) {
                int need = required - cur;
                answer += need;
                cur += need;
                time[i + k] += need; // 여기 need!!
            }
        }
        // cout << cur << ' ';
    }
    
    
    
    return answer;
}