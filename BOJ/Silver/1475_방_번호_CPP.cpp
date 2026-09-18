#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    vector<int> arr;
    arr.resize(10);
    int mx = 0;
    double sum = 0;
    int a = 0;

    cin >> n;

    while(n > 0){
        int div = 0;
        div = n % 10;
        n /= 10;
        arr[div]++;
    }

    // cout << cnt << ' ' << s;

    for(int i = 0; i < 10; i++){
        if(i == 6 || i == 9){
            sum += arr[i];
        }else{
            mx = max(mx, arr[i]);
        }
    }

    a = sum * 10 / 2;
    // cout << a << '\n';

    if(a%10 != 0){
        a /= 10;
        a++;
    }else{
        a /=10;
    }

    // cout << mx << ' ' << a << '\n';
    cout << max(mx, a);


    return 0;
}