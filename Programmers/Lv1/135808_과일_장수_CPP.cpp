#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(int k, int m, vector<int> score) {
    int answer = 0;
    bool check = true;
    sort(score.begin(), score.end());
    
    for(int i = score.size()-1; i >= 0; i-=m){
        int mn = 10;
        for(int j = i; j > i - m; j--){
            if(j < 0){
                check = false;
                break;
            } 
            mn = min(mn, score[j]);
            // cout << score[j] << ' ';
        }
        // cout << '\n';
        if(check == false) break;
        answer += mn * m;
    }
    
    return answer;
}