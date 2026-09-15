#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(vector<int> bandage, int health, vector<vector<int>> attacks) {
    int answer = 0;
    
    int nowtime = 1;
    int nowhp = health;
    int succesetime = 0;
    int uphp = 0;
    int succesehp = 0;
    int hptime = 0;
    int time = 0;
    int idx = 0;
    
    time = attacks[attacks.size() - 1][0];
    succesehp = bandage[2];
    uphp = bandage[1];
    hptime = bandage[0];
    
    while(nowtime <= time){
        // cout << nowhp << ' ';
        // cout << nowtime << ' ';
        
        if(nowtime == attacks[idx][0]){
            nowhp -= attacks[idx][1];
            if(nowhp <= 0) break;
            idx++;
            succesetime = 0;
            nowtime++;
            continue;
        }
        
        if(nowhp == health){
            nowtime++;
            succesetime++;
            continue;
        }
        
        nowhp += uphp;
        if(nowhp > health){
            nowhp = health;
        }
        succesetime++;
        if(succesetime == hptime){
            if(nowhp + succesehp <= health){
                nowhp += succesehp;
            }else{
                nowhp = health;
            }
            succesetime = 0;
        }
        
        nowtime++;
    }
    cout << nowhp;
    answer = (nowhp > 0) ? nowhp : -1;
    
    return answer;
}