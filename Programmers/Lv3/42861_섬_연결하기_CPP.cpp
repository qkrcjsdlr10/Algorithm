#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> parant;

bool cmp(vector<int> a, vector<int> b){
    return a[2] < b[2];
}

int find(int x){
    if(parant[x] == x) return x;
    return parant[x] = find(parant[x]);
}

void unite(int a, int b){
    a = find(a);
    b = find(b);
    
    if(a != b){
        parant[b] = a;
    }
}

int solution(int n, vector<vector<int>> costs) {
    int answer = 0;
    
    parant.resize(n);
    
    sort(costs.begin(), costs.end(), cmp);
    
    for(int i = 0; i < n; i++){
        parant[i] = i;
    }
    
    for(int i = 0; i < costs.size(); i++){
        if(find(costs[i][0]) != find(costs[i][1])){
            answer += costs[i][2];
            unite(costs[i][0], costs[i][1]);
        }
    }
    
    return answer;
}