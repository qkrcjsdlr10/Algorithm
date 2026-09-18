#include <bits/stdc++.h>
using namespace std;

int dp[100];

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int tc = 0;

    cin >> tc;

    for(int test = 0; test < tc; test++){
        int n;
        cin >> n;

        dp[1] = 1;
        dp[2] = 2;
        dp[3] = 4;

        for(int i = 4; i <= n; i++){
            dp[i] = dp[i-3] + dp[i-2] + dp[i-1];
        }

        cout << dp[n] << '\n';
    }


    return 0;
}