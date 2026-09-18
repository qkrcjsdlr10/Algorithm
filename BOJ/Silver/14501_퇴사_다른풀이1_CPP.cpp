#include<bits/stdc++.h>
using namespace std;


vector<vector<int>> arr;
int n = 0;
int money = 0;
int mxmoney = 0;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    
    cin >> n;
    int cnt = 0;
    int day = 0;
    vector<int> dp;
    dp.resize(n+1);
    

    arr.resize(n, vector<int> (2));

    for(int i = 0; i < n; i++){
        cin >> arr[i][0] >> arr[i][1]; 
    }

    if(arr[n-1][0] == 1){
        money = arr[n-1][1];
        dp[n-1] = money;
    }


    for(int i = n-2; i >= 0; i--){
        money = 0;
        if(i + arr[i][0] <= n){
            // money += arr[i][1] + dp[i + arr[i][1]];
            money += dp[i+arr[i][0]] + arr[i][1];
            // cout<< i << ' ' << i + arr[i][0] << ' ' << dp[i + arr[i][0]] << ' ';
        }
        // cout << money << ' ' << dp[i+1] << '\n';
        money = max(money, dp[i+1]);
        dp[i] = money;
    }
    // cout << '\n';
    // for(int i = 0; i < n; i++){
    //     cout << dp[i] << ' ';
    // }

    cout << dp[0];

    return 0;
}
//0  1  2  3  4  5  6  7  8  9 
//90 80 70 60 50 40 30 30  0  0