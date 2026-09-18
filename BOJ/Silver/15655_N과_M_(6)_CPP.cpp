#include <bits/stdc++.h>
using namespace std;

int n = 0;
int sz = 0;
vector<int> ans;
vector<int> arr;
vector<bool> vis;
void dfs(int cnt){
    if(cnt == sz){
        for(int i = 0; i < ans.size(); i++){
            cout << ans[i] << ' ';
        }
        cout << '\n';
        return;
    }

    
    for(int i = 0; i < n; i++){
        if(ans[cnt-1] < arr[i]){
            ans[cnt] = arr[i];
        }else if(cnt == 0){
            ans[cnt] = arr[i];
        }else{
            continue;
        }
        
        dfs(cnt+1);
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    
    cin >> n >> sz;
    ans.resize(sz);
    arr.resize(n);

    for(int i = 0; i < n; i++){
        cin >> arr[i];
    }

    sort(arr.begin(), arr.end());

    dfs(0);


    return 0;
}