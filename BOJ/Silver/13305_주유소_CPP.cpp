#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n = 0;

    vector<int> distance;
    vector<long long> cost;
    long long nowCost = 0;
    long long res = 0;


    cin >> n;

    distance.resize(n-1);
    cost.resize(n);

    for(int i = 0; i < n-1; i++){
        cin >> distance[i];
    }

    for(int i = 0; i < n; i++){
        cin >> cost[i];
    }

    int idx = 0;

    long long minCost = cost[0];

    for(int i = 0; i < n-1; i++){
        minCost = min(minCost, cost[i]);
        res += minCost * distance[i];
    }
    
    cout << res;

    return 0;
}