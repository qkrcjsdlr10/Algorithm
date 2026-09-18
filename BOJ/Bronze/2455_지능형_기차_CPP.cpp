#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 4;
    int sum = 0;
    int now = 0;

    vector<int> arr;

    for(int i = 0; i < n; i++){
        int a = 0;
        int b = 0;
        
        cin >> a >> b;
        now += b - a;
        sum = max(sum, now);
    }

    cout << sum;
    return 0;
}