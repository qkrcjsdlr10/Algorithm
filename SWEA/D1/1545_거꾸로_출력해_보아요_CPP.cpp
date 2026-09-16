#include <bits/stdc++.h>
using namespace std;

void recursion(int n){
    cout << n << " ";

    if(n > 0){
        recursion(n-1);
    }
}

int n;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;

    recursion(n);


    return 0;
}