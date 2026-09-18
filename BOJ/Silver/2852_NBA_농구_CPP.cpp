#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;

    cin >> tc;

    vector<string> score;
    vector<int> team;

    int team1 = 0;
    int team2 = 0;

    int timem1 = 0;
    int timem2 = 0;
    int times1 = 0;
    int times2 = 0;

    int bm = 0;
    int bs = 0;
    int nm = 0;
    int ns = 0;

    string sbm = "";
    string sbs = "";
    string snm = "";
    string sns = "";

    string res1 = "";
    string res2 = "";


    for(int i = 0; i < tc; i++){
        for(int j = 0; j < 2; j++){
            if(j == 0){
                int a = 0;
                cin >> a;
                team.push_back(a);
            }else{
                string b = "";
                cin >> b;
                score.push_back(b);
            }
        }
    }

    bool check = false;

    if(team[0] == 1){
        team1++;
    }else{
        team2++;
    }

    string beforet = score[0];

    for(int i = 1; i < tc; i++){
        bm = 0;
        bs = 0;
        nm = 0;
        ns = 0;

        sbm = "";
        sbs = "";
        snm = "";
        sns = "";

        beforet = score[i-1];

        for(int j = 0; j < beforet.size(); j++){
            if(j == 0 || j == 1){
                sbm += beforet[j];
                snm += score[i][j];
            }else if(j == 3 || j == 4){
                sbs += beforet[j];
                sns += score[i][j];
            }else{
                continue;
            }
        }

        bm = stoi(sbm);
        bs = stoi(sbs);
        nm = stoi(snm);
        ns = stoi(sns);

        if(team1 > team2){
            if(ns < bs){
                times1 += 60 + ns - bs;
                timem1 += nm - bm - 1;
            }else{
                timem1 += nm - bm;
                times1 += ns - bs;
            }
        }else if(team1 < team2){
            if(ns < bs){
                times2 += 60 + ns - bs;
                timem2 += nm - bm - 1;
            }else{
                timem2 += nm - bm;
                times2 += ns - bs;
            }
        }

        if(team[i] == 1){
            team1++;
        }else{
            team2++;
        }
    }

    int lm = stoi(score.back().substr(0, 2));
    int ls = stoi(score.back().substr(3, 2));

    if (team1 > team2) {
        if (ls > 0) {
            timem1 += 48 - lm - 1;
            times1 += 60 - ls;
        } else {
            timem1 += 48 - lm;
        }
    } else if (team2 > team1) {
        if (ls > 0) {
            timem2 += 48 - lm - 1;
            times2 += 60 - ls;
        } else {
            timem2 += 48 - lm;
        }
    }


    timem1 += times1 / 60;
    times1 %= 60;

    timem2 += times2 / 60;
    times2 %= 60;

    

    if(timem1 < 10){
        res1 += string("0") + to_string(timem1);
    }else{
        res1 += to_string(timem1);
    }

    if(times1 < 10){
        res1 += ":" + string("0") + to_string(times1);
    }else{
        res1 += ":" + to_string(times1);
    }


    if(timem2 < 10){
        res2 += string("0") + to_string(timem2);
    }else{
        res2 += to_string(timem2);
    }

    if(times2 < 10){
        res2 += ":" + string("0") + to_string(times2);
    }else{
        res2 += ":" + to_string(times2);
    }

    cout << res1 << '\n' << res2;

    return 0;
}