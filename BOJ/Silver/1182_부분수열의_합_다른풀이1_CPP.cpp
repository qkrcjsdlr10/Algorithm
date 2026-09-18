#include <bits/stdc++.h>
using namespace std;

int n = 0, s = 0;
int res = 0;
int answer = 0;

vector<int> arr;

void dfs(int k, int tot){
    if(tot == s) answer++;

    for(int i = k; i < n; i++){
        dfs(i + 1, tot + arr[i]);
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n >> s;
    arr.resize(n);

    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }

    dfs(0, 0);
    if(s == 0) answer--;

    cout << answer;

    return 0;
}