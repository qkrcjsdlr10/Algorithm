#include <bits/stdc++.h>
using namespace std;

vector<int> arr;
vector<int> prt;

int n = 0;
int k = 0;

void dfs(int depth){
    if(depth == k){
        for(auto p : prt){
            cout << p << ' ';
        }
        cout << '\n';
        return;
    }

    for(int i = 0; i < n; i++){
        if(depth == 0){
            prt.push_back(arr[i]);
        }else if(prt.back() <= arr[i]){
            prt.push_back(arr[i]);
        }else{
            continue;
        }
        dfs(depth + 1);
        prt.pop_back();
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> k;

    arr.resize(n);

    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }
    sort(arr.begin(), arr.end());
    // cout << prt.size();

    dfs(0);

    return 0;
}