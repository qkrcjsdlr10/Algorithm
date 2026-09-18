#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int size = 0;
    cin >> size;
    string num = "";
    cin >> num;
    int sum = 0;

    for(int i = 0; i < size; i++){
        int a = int(num[i]) - '0';
        sum += a;
    }

    cout << sum;
}