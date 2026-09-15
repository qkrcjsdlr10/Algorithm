#include <string>
#include <vector>
#include <iostream>

using namespace std;

const int MOD = 1234567;

long long func(long long n){
    if (n == 0) return 0;
    if (n == 1) return 1;
    
    vector<long long> arr;
    
    arr.resize(n+1);
    
    arr[0] = 0;
    arr[1] = 1;
    
    
    for(int i = 2; i <= n; i++){
        arr[i] = (arr[i - 1]  + arr[i -2]) % MOD;   
        // cout << " " << arr[i] << " ";
    }
    
    return arr[n];
}

int solution(int n) {
    long long answer = 0;
    
    answer = func(n);
    
    return answer;
}