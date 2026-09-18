#include <bits/stdc++.h>

using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    cin >> n;

    vector<long long> dp(n+5);
    dp[1] = 1;
    dp[2] = 2;
    dp[3] = 3;

    for(int i = 4; i <= n; i++){
        dp[i] = (dp[i-2] + dp[i-1]) % 10007;
    }
    
    cout << dp[n] % 10007;

    return 0;
}