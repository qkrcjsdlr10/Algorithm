#include <bits/stdc++.h>
using namespace std;

vector<int> arr;
vector<bool> sel;
vector<int> prt;

void dfs(int k, int n){
    if(k == n){
        for(int i = 0; i < prt.size(); i++){
            cout << prt[i] << ' ';
        }
        cout << '\n';
        // cout << k;
        return;
    }

    for(int i = k; i < arr.size(); i++){
        if(!sel[i]){
            if (!prt.empty() && arr[i] < prt.back()) continue;
            prt.push_back(arr[i]);
            sel[i] = true;
            dfs(k+1, n);
            prt.pop_back();
            sel[i] = false;
        }
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    while(true){
        int cnt = 0;
        int n = 0;
        cin >> n;
        if(n == 0){
            break;
        }
        arr.assign(0, 0);
        for(int i = 0; i < n; i++){
            int a = 0;
            cin >> a;
            arr.push_back(a);
        }
    
        sel.assign(arr.size(), 0);
    
        sort(arr.begin(), arr.end());
        dfs(0, 6);
        cout << '\n';
    }
    
    
    
    return 0;
}