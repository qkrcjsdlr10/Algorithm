#include<bits/stdc++.h>
using namespace std;

int n = 0, k = 0;
int res = 0;
vector<int> arr;
vector<bool> vis;
vector<int> arr1;

void dfs(int start, int sum){
    if(sum == k){
        res++;
    }
    // int b = 0;
    // for(auto a : arr1){
    //     b += a;
    // }
    // if(b == k) res++;

    // for(int i = 0; i < n; i++){
    //     if(vis[i]) continue;
    //     vis[i] = true;
    //     arr1.push_back(arr[i]);
    //     for(auto a : arr1){
    //         cout << a << ' ';
    //     }
    //     cout << '\n';
    //     dfs(depth + 1, sum + arr[i]);
    //     vis[i] = false;
    //     arr1.pop_back();
    // }
    for (int i = start; i < n; i++) {
        arr1.push_back(arr[i]);

        dfs(i + 1, sum + arr[i]);

        arr1.pop_back();
    }
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;
    for(int t = 1; t <= tc; t++){
        n = 0;
        k = 0;
        res = 0;
        

        cin >> n >> k;
        vis.assign(n, 0);
        arr.assign(0, 0);
        
        for(int i = 0; i < n; i++){
            int num = 0;
            cin >> num;
            arr.push_back(num);
        }

        dfs(0, 0);

        cout << '#' << t << ' ' << res << '\n';
    }
    
    
    return 0;
}