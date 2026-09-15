#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int n) {
    int answer = 0;
    
    vector<int> dp;
    dp.resize(n+1);
    dp[1] = 1;
    if(n == 1) return 1;
    dp[2] = 2;
    
    for(int i = 3; i <= n; i++){
        dp[i] = (dp[i-1] + dp[i-2]) % 1000000007;
    }
    
    // for(auto d :dp){
    //     cout << d << ' ';
    // }
    answer = dp[n];    
    return answer;
}