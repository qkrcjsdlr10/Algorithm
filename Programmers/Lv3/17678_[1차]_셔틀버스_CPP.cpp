#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

string solution(int n, int t, int m, vector<string> timetable) {
    string answer = "";
    
    vector<int> time;
    
    for(auto t : timetable){
        int h = stoi(t.substr(0, 2));
        int m = stoi(t.substr(3, 2));
        int temp = (h * 60) + m;
        time.push_back(temp);
    }
    
    sort(time.begin(), time.end());
    
    // for(auto t : time){
    //     cout << t << ' ';
    // }
    
    int shuttle = 540;
    int idx = 0;
    int cnt = 0;
    
    for(int i = 0; i < n; i++){
        cnt = 0;
        while(idx < time.size() && cnt < m && time[idx] <= shuttle){
            idx++;
            cnt++;
        }
        if(i != n - 1) shuttle += t;
    }
    
    cout << cnt;
    
    int res;
    if(cnt < m){
        res = shuttle;
    }else{
        res = time[idx - 1] - 1;
    }
    int hh = res / 60;
    int mm = res % 60;
    if(hh < 10){
        answer = "0" + to_string(hh) + ":";
    }else{
        answer = to_string(hh) + ":";
    }
    if(mm < 10){
        answer += "0" + to_string(mm);
    }else{
        answer += to_string(mm);
    }
    
    
    
    return answer;
}