#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, sum, remainder;
    cin >> n;

    while(n != 0){
        remainder = n%10;
        sum += remainder;
        n /= 10;
    }

    cout << sum;
    
    return 0;
}