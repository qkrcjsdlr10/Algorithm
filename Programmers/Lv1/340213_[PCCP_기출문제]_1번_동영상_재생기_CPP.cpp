#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;
int cur = 0;
int totallen = 0;
int opstart = 0;
int opend = 0;

int timetoint(string time){
    int hh = stoi(time.substr(0, 2));
    int mm = stoi(time.substr(3, 2));
    
    int t = hh * 60;
    t += mm;
    
    return t;
}

string inttotime(int t){
    string temp = "";
    
    int h = t / 60;
    int m = t % 60;
    if(h < 10){
        temp += '0' + to_string(h);
    }else{
        temp += to_string(h);
    }
    
    temp += ':';
    
    if(m < 10){
        temp += '0' + to_string(m);
    }else{
        temp += to_string(m);
    }
    
    return temp;
}

int command(int cur, string cmd){
    if(cmd == "next"){
        if(cur + 10 <= totallen){
            cur += 10;
        }else{
            cur = totallen;
        }
    }else if(cmd == "prev"){
        if(cur - 10 >= 0){
            cur -= 10;
        }else{
            cur = 0;
        }
    }
    
    return cur;
}

string solution(string video_len, string pos, string op_start, string op_end, vector<string> commands) {
    string answer = "";
    
    cur = timetoint(pos);
    totallen = timetoint(video_len);
    opstart = timetoint(op_start);
    opend = timetoint(op_end);
    cout << cur << ' ';
    
    if(cur >= opstart && cur <= opend){
        cur = opend;
    }
    
    for(auto cmd : commands){
        cur = command(cur, cmd);
        if(cur >= opstart && cur <= opend){
            cur = opend;
        }
        cout << cur << ' ';
    }
    
    answer = inttotime(cur);
    
    return answer;
}