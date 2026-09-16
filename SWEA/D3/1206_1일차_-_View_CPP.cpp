#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    for(int t = 1; t <= 10; t++){
        int sz = 0;
        long long res = 0;
        cin >> sz;
    
        vector<int> arr;
    
        arr.assign(sz, 0);
    
        for(int i = 0; i < sz; i++){
            cin >> arr[i];
        }
    
        for(int i = 2; i < sz-2; i++){
            int rmx = 0;
            int lmx = 0;
            int mx = 0;
    
            lmx = max(arr[i-2], arr[i-1]);
            rmx = max(arr[i+1], arr[i+2]);
            mx = max(lmx, rmx);
    
            if(arr[i] > mx){
                res += arr[i] - mx;
            }
    
        }
        cout << '#' << t << ' ' << res << '\n';
    }

    

    return 0;
}