#include <bits/stdc++.h>
using namespace std;

int sum;

void func(int num){
    if(num == 0) return;

    sum += num;
    func(--num);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int num = 0;
    cin >> num;

    func(num);

    cout << sum;


    return 0;
}