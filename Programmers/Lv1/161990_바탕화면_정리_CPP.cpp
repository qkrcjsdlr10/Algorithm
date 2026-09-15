#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(vector<string> wallpaper) {
    vector<int> answer;
    
    int lux = 1e9, luy = 1e9, rdx = 0, rdy = 0;
    
    int n = wallpaper.size();
    int m = wallpaper[0].size();
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(wallpaper[i][j] == '#'){
                rdx = max(rdx, i);
                lux = min(lux, i);
                
                luy = min(luy, j);
                rdy = max(rdy, j);
            }
        }
    }
    
    for(auto wa : wallpaper){
        for(auto w : wa){
            cout << w << ' ';
        }
        cout << '\n';
    }
    
    cout << lux << ' ' << luy << ' ' << rdx+1 << ' ' << rdy+1;
    answer.push_back(lux);
    answer.push_back(luy);
    answer.push_back(rdx+1);
    answer.push_back(rdy+1);
    return answer;
}