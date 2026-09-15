#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

class cmp{
    public:
    bool operator()(vector<int> a, vector<int> b){
        return a[1] > b[1];
    }
};

int solution(vector<vector<int>> jobs) {
    int answer = 0;
    int res = 0;
    int time = 0;
    int idx = 0;
    
    priority_queue<vector<int>, vector<vector<int>>, cmp> pq;
    
    sort(jobs.begin(), jobs.end());
    
    while(idx < jobs.size() || !pq.empty()){
        while(idx < jobs.size() && jobs[idx][0] <= time) {
            pq.push(jobs[idx]);
            idx++;
        }

        if(pq.empty()) {
            time = jobs[idx][0];
            continue;
        }
        auto cur = pq.top(); pq.pop();
        
        time += cur[1];
        
        res += time - cur[0];
        // cout << res << ' ';
    }
    
    cout << res;
    
    answer = res / jobs.size();
    
    return answer;
}