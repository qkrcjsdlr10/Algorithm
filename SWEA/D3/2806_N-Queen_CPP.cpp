#include <bits/stdc++.h>
using namespace std;


int n, cnt, ts;
bool isused1[40], isused2[40], isused3[40];


void func(int k){
    if(k == n){
        cnt++;
        return;
    }

    for(int i = 0; i < n; i++){
        if(isused1[i] || isused2[i+k] || isused3[k-i+n-1]) continue;
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
    cin >> ts;
    for(int i = 1; i <= ts; i++){
        cin >> n;

        func(0);

        cout << '#' << i << ' ' << cnt << '\n';
        cnt = 0;
    }
    

    return 0;
}