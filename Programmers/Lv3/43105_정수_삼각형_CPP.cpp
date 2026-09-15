#include <string>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int solution(vector<vector<int>> triangle) {
    int answer = 0;
    
    vector<vector<int>> dp(triangle.size(), vector<int>(triangle.size()));
    
    // for(auto ar : triangle){
    //     for(auto a : ar){
    //         cout << a << ' ';
    //     }
    //     cout << '\n';
    // }
    dp[0][0] = triangle[0][0];
    
    for(int i = 1; i < triangle.size(); i++){
        for(int j = 0; j < i+1; j++){
            dp[i][j] = max(triangle[i][j] + dp[i-1][j], triangle[i][j] + dp[i-1][j-1]);
            // cout << dp[i][j] << ' ';
        }
        // cout << '\n';
    }
    
    answer = *max_element(dp[triangle.size()-1].begin(), dp[triangle.size()-1].end());
    
    return answer;
}