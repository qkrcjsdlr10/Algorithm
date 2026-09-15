#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(int n, vector<int> money) {
    int answer = 0;
    
    vector<long long> dp(n + 1);
    
    sort(money.begin(), money.end());
    
    dp[0] = 1;
    
    for (int coin : money) {
        for (int i = coin; i <= n; i++) {
            dp[i] += dp[i - coin];
            dp[i] %= 1000000007;
        }
    }
    
    answer = dp[n] % 1000000007;
    
    return answer;
}