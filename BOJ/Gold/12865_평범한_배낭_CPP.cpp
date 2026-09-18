#include <bits/stdc++.h>
using namespace std;

int n, k;
int mxWeight, mxPrice;
vector<vector<int>> arr;

vector<vector<int>> dp;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> k;

    // vector<vector<int>> arr(n, vector<int>(n));
    arr.resize(n+1, vector<int>(2));
    dp.resize(n+1, vector<int>(k+1));

    for(int i = 1; i <= n; i++){
        cin >> arr[i][0] >> arr[i][1];
    }

    for(int i = 1; i <= n; i++){
        for(int j = 1; j <= k; j++){
            if(arr[i][0] > j){
                dp[i][j] = dp[i-1][j];
            }else{
                dp[i][j] = max(dp[i-1][j], dp[i-1][j-arr[i][0]] + arr[i][1]);
            }
        }
    }

    cout << dp[n][k];

    return 0;
}