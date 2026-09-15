#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(vector<int> a) {
    int answer = 2;
    int n = a.size();
    
    vector<int> leftmin(n);
    vector<int> rightmin(n);
    leftmin[0] = a[0];
    rightmin[n-1] = a[n-1];
    
    
    for(int i = 1; i < n-1; i++){
        leftmin[i] = min(leftmin[i-1], a[i]);
    }
    
    for(int i = n - 2; i >= 0; i--){
        rightmin[i] = min(rightmin[i+1], a[i]);
    }
    
    for(int i = 1; i < n - 1; i++){
        bool check = false;
        if(a[i] <= leftmin[i - 1]){
            check = true;
        }
        if(!check){
            if(a[i] <= rightmin[i + 1]){
                check = true;
            }
        }
        if(check) answer++;
    }
    
    return answer;
}