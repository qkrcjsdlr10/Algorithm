#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int m = 0;
    int idx1 = 0;
    int idx2 = 0;

    vector<int> arr1;
    vector<int> arr2;
    vector<int> res;

    cin >> n >> m;

    arr1.resize(n);
    arr2.resize(m);
    res.resize(n+m);

    for(int i = 0; i < n; i++){
        cin >> arr1[i];
    }

    for(int i = 0; i < m; i++){
        cin >> arr2[i];
    }

    for(int i = 0; i < n + m; i++){
        if(idx1 >= n){
            res[i] = arr2[idx2];
            idx2++;
            continue;
        }else if(idx2 >= m){
            res[i] = arr1[idx1];
            idx1++;
            continue;
        }
        if(arr1[idx1] < arr2[idx2]){
            res[i] = arr1[idx1];
            idx1++;
        }else{
            res[i] = arr2[idx2];
            idx2++;
        }
        // cout << idx1 << ' ' << idx2 << '\n';
    }

    // cout << idx1 << ' ' << idx2 << '\n';

    for(int i = 0; i < n + m; i++){
        cout << res[i] << ' ';
    }


    return 0;
}
