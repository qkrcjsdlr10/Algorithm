#include <string>
#include <vector>
#include <iostream>

using namespace std;

long long toDecimal(const vector<int>& digits, int n) {
    long long x = 0;
    for (int d : digits) {
        x = x * n + d;   // k진수 -> 10진수
    }
    return x;
}

long long isPrime(long long x){
    if(x < 2) return 0;
    if(x == 2) return 1;
    if(x % 2 == 0) return 0;
    for(long long i = 3; i * i <= x; i+=2){
        if(x % i == 0) return 0;
    }
    return 1;
}

int solution(int n, int k) {
    int answer = 0;
    vector<int> sub;
    vector<int> arr;
    vector<vector<int>> res;
    
    while(n > 0){
        int div = 0;
        div = n % k;
        n /= k;
        arr.push_back(div);
    }
    
    int a = 0;
    int cnt = 1;
    
    for(int i = arr.size()-1; i >= 0; i--){
        if(arr[i] == 0){
            if(sub.size() != 0){
                res.push_back(sub);
                sub.clear();
            }
        }else{
            sub.push_back(arr[i]);
        }
    }
    if(sub.size() != 0)
        res.push_back(sub);
    // cout << res.size();
    for(auto r : res){
        long long a = toDecimal(r, 10);
        answer += isPrime(a);
        // cout << a << ' ' << isPrime(a) << ' ' << answer << '\n';
        // cout << '\n';
    }
    
    
    return answer;
}