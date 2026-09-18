#include <bits/stdc++.h>
using namespace std;

int n, k;
int W[101];
int V[101];
int dp[101][100001];

int func(int i, int w){
    if(dp[i][w] > 0) return dp[i][w];
    if(i == n) return 0;

    int n1 = 0;
    if(w + W[i] <= k){
        n1 = V[i] + func(i + 1, w + W[i]);
    }
    
    int n2 = func(i + 1, w);
    return dp[i][w] = max(n1, n2);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> k;

    for(int i = 0; i < n; i++){
        cin >> W[i] >> V[i];
    }
    int res = 0;
    res = func(0, 0);
    cout << res;

    return 0;
}