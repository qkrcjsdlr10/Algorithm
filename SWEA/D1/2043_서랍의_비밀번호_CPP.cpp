#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int num = 0;
    int num1 = 0;
    int res = 0;
    cin >> num >> num1;

    for(int i = num1; i <= num; i++){
        res++;
    }

    cout << res;

    return 0;
}