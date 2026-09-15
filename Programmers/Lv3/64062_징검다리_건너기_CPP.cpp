#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

bool canjump(vector<int> &stones, int mid, int k){
    int cnt = 0;
    
    for(auto x : stones){
        if(x < mid) cnt++;
        else cnt = 0;
        
        if(cnt >= k) return false;
    }
    return true;
}

int solution(vector<int> stones, int k) {
    int answer = 0;
    int n = stones.size();
    int left = 1;
    int right = *max_element(stones.begin(), stones.end());
    
    while(left <= right){
        int mid = (left + right) / 2;
        
        bool jumpok = canjump(stones, mid, k);
        if(jumpok){
            answer = mid;
            left = mid + 1;
        }else{
            right = mid - 1;
        }
    }
    
    return answer;
}