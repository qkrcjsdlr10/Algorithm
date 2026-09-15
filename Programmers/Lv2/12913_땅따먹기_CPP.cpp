#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<vector<int> > land)
{
    int answer = 0;

    vector<vector<int>> dp;
    
    dp.resize(land.size(), vector<int>(land[0].size()));
    
    dp[0] = land[0];
    
    for(int i = 1; i < dp.size(); i++){
        for(int j = 0; j < dp[0].size(); j++){
            int mx = 0;
            for(int k = 0; k < dp[0].size(); k++){
                if(k == j) continue;
                mx = max(dp[i-1][k], mx);
            }
            dp[i][j] = mx + land[i][j];
        }
    }
    answer = *max_element(dp[dp.size()-1].begin(), dp[dp.size()-1].end());
    
    return answer;
}