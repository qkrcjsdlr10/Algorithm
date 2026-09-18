#include <bits/stdc++.h>
using namespace std;

int arr[10000];
bool res[10000];
int idx, sum, d;
int a;

void func(int n){
    idx = 0;
    sum = 0;
    d = n;
    if(n >= 10000) return;

    while(d>0){
        arr[idx] = d%10;
        d /= 10;
        idx++;
    }

    for(int i = 0; i < idx; i++){
        sum += arr[i];
    }
    sum += n;
    res[sum] = true;

    func(n+1);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    func(0);

    for(int i = 1; i < 10000; i++){
        if(!res[i]) cout << i << '\n';
    }

    return 0;
}