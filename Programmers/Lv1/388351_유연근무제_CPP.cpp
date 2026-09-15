#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(vector<int> schedules, vector<vector<int>> timelogs, int startday) {
    int answer = 0;
    
    for(int i = 0; i < schedules.size(); i++){
        bool check = false;
        string schedule = to_string(schedules[i]);
        int endtime = 0;
        // cout << schedule << ' ';
        if(schedule.size() == 4){
            endtime += stoi(schedule.substr(0, 2)) * 60;
            endtime += stoi(schedule.substr(2));
        }else{
            endtime += stoi(schedule.substr(0, 1)) * 60;
            endtime += stoi(schedule.substr(1));
        }
        endtime += 10;
        // cout << endtime << '\n';
        for(int j = 0; j < timelogs[i].size(); j++){
            if(check) break;
            // cout << (startday + j) % 7 << ' ';
            int nowday = (startday + j - 1) % 7;
            if(nowday == 5 || nowday == 6) continue;
            // cout << nowday << ' ';
            string time = to_string(timelogs[i][j]);
            int t = 0;
            if(time.size() == 4){
                t += stoi(time.substr(0, 2)) * 60;
                t += stoi(time.substr(2));
            }else{
                t += stoi(time.substr(0, 1)) * 60;
                t += stoi(time.substr(1));
            }
            if(t > endtime){
                check = true;
            }
        }
        if(!check) answer++;
    }
    
    return answer;
}