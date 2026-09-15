#include <iostream>
#include <string>
#include <bits/stdc++.h>

using namespace std;
int solution(string s)
{
    int answer = 1;
    int n = s.size();
    vector<vector<int>> dp(n, vector<int>(n));
    
    for(int i = 0; i < n; i++){
        dp[i][i] = 1;
    }
    
    for(int i = 0; i < n-1; i++){
        if(s[i] == s[i+1]){
            dp[i][i+1] = 1;
            answer = 2;
        }
    }
    for(int len = 3; len <= n; len++){
        for(int i = 0; i + len - 1 < n; i++){
            int j = i + len - 1;
            
            if(s[i] == s[j] && dp[i+1][j-1]){
                dp[i][j] = 1;
                answer = max(answer, j - i + 1);
            }
        }
    }
    
    return answer;
}