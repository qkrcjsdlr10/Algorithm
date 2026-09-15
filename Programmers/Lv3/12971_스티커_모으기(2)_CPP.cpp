#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int mx = 0;
int n = 0;

int solution(vector<int> sticker)
{
    int answer = 0;
    n = sticker.size();
    
    vector<int> dp(n);
    dp[0] = sticker[0];
    dp[1] = sticker[0];
    for(int i = 2; i < n-1; i++){
        dp[i] = max(dp[i-1], dp[i-2] + sticker[i]);
    }
    mx = *max_element(dp.begin(), dp.begin() + (n-1));
    dp.assign(n, 0);
    dp[0] = 0;
    dp[1] = sticker[1];
    
    for(int i = 2; i < n; i++){
        dp[i] = max(dp[i-1], dp[i-2] + sticker[i]);
    }
    
    mx = max(mx, *max_element(dp.begin(), dp.begin() + n));
    answer = mx;
    return answer;
}