#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    cin >> n;

    int arr[300] = {0};
    int dp[300][3] = {0};

    for(int i = 1; i <= n; i++){
        cin >> arr[i];
    }

    if(n == 1){
        cout << arr[1];
        return 0;
    }

    dp[1][1] = arr[1];
    dp[1][2] = 0;
    dp[2][1] = arr[2];
    dp[2][2] = arr[1] + arr[2];

    for(int i = 3; i <= n; i++){
        for(int j = 1; j < 3; j++){
            if(j == 1){
                dp[i][1] = max(dp[i-2][1] , dp[i-2][2]) + arr[i];
            }
            if(j == 2){
                dp[i][2] = dp[i-1][1] + arr[i];
            }
        }
    }
    
    int answer = 0;
    answer = max(dp[n][1], dp[n][2]);
    cout << answer;

    return 0;
}