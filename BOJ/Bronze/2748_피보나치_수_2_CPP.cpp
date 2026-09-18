#include <bits/stdc++.h>

using namespace std;

int main(){
    int n = 0;
    cin >> n;

    vector<long long> arr;
    arr.push_back(0);
    arr.push_back(1);

    for(int i = 2; i <= n; i++){
        long long a = arr[i-1] + arr[i-2];
        arr.push_back(a);
    }

    // for(int i = 0; i < n; i++){
    //     cout << arr[i] << ' ';
    // }

    cout << arr[n];

    return 0;
}