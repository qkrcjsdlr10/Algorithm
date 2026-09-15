#include <string>
#include <vector>
#include <iostream>

using namespace std;

long long solution(int n) {
    long long answer = 0;
    
    vector<long long> arr;
    
    arr.resize(n+1);
    arr[1] = 1;
    arr[2] = 2;
    
    for(int i = 3; i <= n; i++){
        arr[i] = (arr[i - 1] + arr[i - 2]) % 1234567;
    }
    
    return arr[n];
}