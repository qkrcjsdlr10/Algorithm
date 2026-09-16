#include <bits/stdc++.h>

using namespace std;

int dp[10];
int arr[100][100];
int isused[100];
int step;
int mx;
int score, mxScore;
int k;
int sum;

void func(int n, int idx){
    if(n == step) return;

    if(sum > k){
        sum = sum - arr[idx][1];
        score -= arr[idx][0];
        return;
    } 

    // score += arr[n][0];
    // sum += arr[n][1];

    for(int i = 0+n; i < step; i++){
        if(!isused[i]){
            score += arr[i][0];
            sum += arr[i][1];
            isused[i] = 1;
            func(n+1, i);
            isused[i] = 0;
        }
    }

    // mx = max(mx, sum);

    // func(n);
    // func(n+1);
    // mx = sum - arr[n][1];
}

void func1(int start, int currentScore, int currentCalorie) {
    if (currentCalorie > k) return;

    if (currentScore > mxScore) {
        mxScore = currentScore;
    }
    
    for (int i = start; i < step; i++) {
        func1(i + 1, currentScore + arr[i][0], currentCalorie + arr[i][1]);
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int T;
    cin >> T;

    for (int tc = 1; tc <= T; tc++) {
        cin >> step >> k;
        mxScore = 0;

        for (int i = 0; i < step; i++) {
            cin >> arr[i][0] >> arr[i][1];
        }

        func1(0, 0, 0);
        cout << "#" << tc << " " << mxScore << '\n';
    }

    

    return 0;
}