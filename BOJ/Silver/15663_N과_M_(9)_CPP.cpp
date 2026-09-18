#include <bits/stdc++.h>
using namespace std;

vector<int> arr;
vector<int> prt;

int n = 0;
int k = 0;
set<vector<int>> st;
vector<bool> vis;

void dfs(int depth){
    if(depth == k){
        if(st.find(prt) == st.end()){
            st.insert(prt);
            for(auto p : prt){
                cout << p << ' ';
            }
            cout << '\n';
        }
        return;
    }

    for(int i = 0; i < n; i++){
        if(vis[i]) continue;
        prt.push_back(arr[i]);
        vis[i] = true;
        dfs(depth + 1);
        prt.pop_back();
        vis[i] = false;
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> k;
    vis.resize(n);
    arr.resize(n);

    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }
    sort(arr.begin(), arr.end());
    // cout << prt.size();

    dfs(0);

    return 0;
}