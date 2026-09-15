#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

int solution(int x, int y, int n) {
    int answer = 0;
    
    vector<int> dp;
    dp.assign(y+1, 1e9);
    int cnt = 0;
    dp[x] = 0;
    
    for(int i = x; i <= y; i++){
        if(dp[i] == 1e9) continue;
        
        if(i * 2 <= y){
            // cout << i*2 << ' ' << dp[i*2] << ' ';
            // cout << i*2 << ' ' << dp[i*2] << ' ';
            dp[i*2] = min(dp[i]+1, dp[i*2]);
            // cout << i << ' ' << dp[x]+1 << ' ' << dp[i*2] << '\n';
        }
        if(i * 3 <= y){
            dp[i*3] = min(dp[i]+1, dp[i*3]);
        }
        if(i + n <= y){
            dp[i+n] = min(dp[i]+1, dp[i+n]);
        }
    }
    // cout << dp[y];
    answer = (dp[y] == 1e9) ? -1 : dp[y];
    
    return answer;
}