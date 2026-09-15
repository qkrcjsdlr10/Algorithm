#include <bits/stdc++.h>

using namespace std;

int solution(int k, vector<int> tangerine) {
    int answer = 0;
    // int arr[10000001];
    sort(tangerine.begin(), tangerine.end());
    int mx = 0;
    for(int i = 0; i < tangerine.size(); i++){
        // cout << tangerine[i] << ' ';
        // arr[tangerine[i]]++;
        if(mx < tangerine[i]){
            mx = tangerine[i];
        }
    }
    
    vector<int> arr;
    arr.resize(mx+1);
    
    
    
    for(int i = 0; i < tangerine.size(); i++){
        // cout << tangerine[i] << ' ';
        arr[tangerine[i]]++;
        
    }
    sort(arr.begin(), arr.end());
    // for(int i = 1; i <= mx; i++){
    //     cout << arr[i] << ' ';
    // }
    
    // cout << mx;
    
    while(k > 0){
        // cout << k << ' ' << arr[mx+1] << '\n';
        k = k - arr[mx];
        answer++;
        mx--;
        
    }
    
    
    // cout << mx;
    
    return answer;
}