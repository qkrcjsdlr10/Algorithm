#include <bits/stdc++.h>
using namespace std;

vector<int> arr;
vector<int> prt;

int n = 0;
int k = 0;

void dfs(int depth){
    if(depth == k){
        for(int i = 0; i < prt.size(); i++){
            cout << prt[i] << ' ';
        }
        cout << '\n';
        return;
    }

    for(int i = 0; i < n; i++){
        prt.push_back(arr[i]);
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

    dfs(0);

    return 0;
}