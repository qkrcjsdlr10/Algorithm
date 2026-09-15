#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(int n, vector<int> stations, int w)
{
    int answer = 0;

    int cur = 0;
    int cover = 2 * w + 1;
    
    for(int i = 0; i < stations.size(); i++){
        int length = (stations[i] - w - 1) - cur;
        if (length > 0) {
            answer += (length + cover - 1) / cover;
        }
        cur = stations[i] + w;
    }
    // cout << cur;
    if(cur < n){
        int length = n - cur;
        answer += (length + cover - 1) / cover;
    }
    return answer;
}