#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int idx = 2;
    int cnt5 = 0;
    int cnt3 = 0;
    cin >> n;

    for(int i = n/5; i >= 0; i--){
        cnt5 = i;
        cnt3 = (n - cnt5 * 5) / 3;
        if((cnt5 * 5 + cnt3 * 3) == n){
            break;
        }else{
            cnt5 = 0;
            cnt3 = 0;
        }
    }

    cout << ((cnt5 + cnt3 != 0) ? cnt5 + cnt3 : -1);

    return 0;
}