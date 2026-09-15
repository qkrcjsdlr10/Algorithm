#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(int n, int s) {
    vector<int> answer;
    
    vector<int> arr;
    if(s / n == 0) return {-1};
    if(s % n == 0){
        for(int i = 0; i < n; i++){
            arr.push_back(s/n);
        }
    }else{
        int div = s % n;
        
        for(int i = 0; i < n; i++){
            arr.push_back(s/n);
        }
        for(int i = n-1; i >= n-div; i--){
            arr[i]++;
        }
    }
    for(auto a : arr){
        cout << a << ' ';
    }
    
    return arr;
}