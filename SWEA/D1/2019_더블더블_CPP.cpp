#include <bits/stdc++.h>
using namespace std;
int res;

void func(int n, int num){
    if(n == 1){
        cout << 1 << ' ';
        n++;
    }
    if(n-1 > num) return;

    
    res *= 2;
    

    cout << res << ' ';
    n++;
    func(n, num);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int num = 0;
    cin >> num;
    res = 1;
    // cout << res;

    func(1, num);

    return 0;
}