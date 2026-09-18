#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int w = 0;
    int l = 0;
    int time = 0;
    int weight = 0;
    queue<pair<int,int>> bridge;
    int finished = 0;
    int idx = 0;
    cin >> n >> w >> l;

    vector<int> arr;
    arr.resize(n);

    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }

    while(finished < n){
        time++;

        if(idx < n && weight + arr[idx] <= l){
            bridge.push({arr[idx], 1});
            weight += arr[idx];
            idx++;
        }
        
        int sz = bridge.size();
        for (int i = 0; i < sz; i++) {
            auto b = bridge.front();
            bridge.pop();
            b.second++;
            bridge.push(b);
        }

        if(!bridge.empty() && bridge.front().second > w){
            weight -= bridge.front().first;
            bridge.pop();  
            finished++;
            // time++;
            // continue;
        }
        if(idx < n && weight + arr[idx] <= l){
            bridge.push({arr[idx], 1});
            weight += arr[idx];
            idx++;
        }
        
        cout << time << ' ' << idx << ' ' << weight << ' ' << bridge.front().first << ' ' << bridge.front().second << '\n';
    }

    
    cout << time;
    
    return 0;
}