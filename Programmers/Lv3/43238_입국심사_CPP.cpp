#include <string>
#include <vector>
#include <bits/stdc++.h>


using namespace std;

long long solution(int n, vector<int> times) {
    long long answer = 0;
    
    long long left = 1;
    long long right = 1LL * (*max_element(times.begin(), times.end())) * n;
    
    
    
    while(left <= right){
        long long sum = 0;
        long long mid = (left + right) / 2;
        
        for(int i = 0; i < times.size(); i++){
            sum += mid / times[i];
        }
        if(sum >= n){
            answer = mid;
            right = mid - 1;
        }else{
            left = mid + 1;
        }
        
    }
    
    return answer;
}