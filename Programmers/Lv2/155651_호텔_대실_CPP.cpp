#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

using namespace std;

bool cmp(const pair<int,int>& a, const pair<int,int>& b) {
    return a.first < b.first;
}


int solution(vector<vector<string>> book_time) {
    int answer = 0;
    vector<pair<int, int>> arr;
    priority_queue<int, vector<int>, greater<int>> pq;
    for(auto t : book_time){
        int stime = 0;
        int etime = 0;
        int idx = 0;
        
        for(auto a : t){
            int mm = 0;
            int hh = 0;
            string str = "";
            for(auto s : a){
                if(s == ':'){
                    hh = stoi(str) * 60;
                    // cout << str << ' ';
                    str = "";
                    continue;
                }
                str += s;
            }
            // cout << str << '\n';
            mm = stoi(str);
            if(idx == 0){
                stime = hh + mm;
                idx++;
            }else{
                etime = hh + mm + 10;
            }
        }
        arr.push_back({stime, etime});
    }
    
    sort(arr.begin(), arr.end(), cmp);
    
    for (auto &p : arr) {
        int start = p.first;
        int end = p.second;

    // 가장 빨리 끝나는 방이 start 전에 비면 재사용
        if (!pq.empty() && start >= pq.top()) {
            pq.pop();
        }

        pq.push(end); // 방 하나 배정
        answer = max(answer, (int)pq.size());
    }
    
    for(auto a : arr){
        cout << a.first/60 << a.first%60 << ' ' << a.second/60 << a.second%60 << '\n';
    }
    
    return answer;
}