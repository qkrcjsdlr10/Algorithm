#include <string>
#include <vector>
#include <deque>
#include <stack>

using namespace std;

int solution(int bridge_length, int weight, vector<int> truck_weights) {
    int answer = 0;
    int w = truck_weights[0];
    int idx = 1;
    deque<pair<int, int>> q;
    // pair<int, int> p;
    q.push_back({truck_weights[0], 0});
    // answer++;
    
    while(!q.empty() || idx < truck_weights.size()){
        for(auto &a : q){
            a.second++;
        }
        
        while(!q.empty() && q.front().second >= bridge_length){
            w -= q.front().first;
            q.pop_front();
        }
        
        if(idx < truck_weights.size() && w + truck_weights[idx] <= weight){
            q.push_back({truck_weights[idx], 0});
            w += truck_weights[idx];
            idx++;
        }
        
        answer++;
    }
    answer++;
    return answer;
}