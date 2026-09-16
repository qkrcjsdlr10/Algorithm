#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        int n = 0, h = 0;
        int mx = 0;
        cin >> n >> h;
        vector<int> arr;
        arr.assign(n, 0);

        for(int i = 0; i < n; i++){
            cin >> arr[i];
            mx += arr[i];
        }

        vector<vector<int>> dp;
        dp.assign(n + 1, vector<int>(mx + 1, 0));

        dp[0][0] = 1;
        dp[1][arr[0]] = 1;

        for(int i = 0; i < n; i++){
            for(int j = 0; j <= mx; j++){
                if(dp[i][j] == 1){
                    dp[i + 1][j] = true;
                    if(j + arr[i] <= mx){
                        dp[i + 1][j + arr[i]] = true;
                    }
                }
            }
        }
        
        for(int i = h; i <= mx; i++){
            if(dp[n][i] == 1){
                cout << '#' << t << ' ' << i - h << '\n';
                break;
            }
        }
    }

    
    
    return 0;
}