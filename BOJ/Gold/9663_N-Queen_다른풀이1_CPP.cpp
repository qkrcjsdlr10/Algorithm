#include <bits/stdc++.h>
using namespace std;


int n, cnt;
bool isused1[40], isused2[40], isused3[40];


void func(int k){
    if(k == n){
        cnt++;
        return;
    }

    for(int i = 0; i < n; i++){
        if(isused1[i] || isused2[k+i] || isused3[k-i+n-1]) continue;
        isused1[i] = 1;
        isused2[i+k] = 1;
        isused3[k-i+n-1] = 1;
        func(k+1);
        isused1[i] = 0;
        isused2[i+k] = 0;
        isused3[k-i+n-1] = 0;
    }

}


int main(){

    cin >> n;

    func(0);

    cout << cnt;

    return 0;
}