#include <bits/stdc++.h>
using namespace std;

int t, sum;
int arr[10];


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    cin >> t;

    for(int i = 1; i <= t; i++){

        for(int i = 0; i < 10; i++){
            cin >> arr[i];
        }

        for(int i = 0; i < 10; i++){
            if(arr[i] % 2 == 1){
                sum += arr[i];
            }
        }
        cout << '#' << i << ' ' << sum << '\n';
        sum = 0;
    }
    return 0;
}