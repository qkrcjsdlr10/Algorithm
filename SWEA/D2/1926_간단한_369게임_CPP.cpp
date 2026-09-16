#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    cin >> n;
    int num = 1;

    while(num <= n){

        int cnt = 0;
        int b = num;

        while(b > 0){
            int div = 0;
            int a = 0;
            a = b % 10;
            b /= 10;
            if(a % 3 == 0 && a != 0){
                cnt++;
            }
        }

        if(cnt == 0){
            cout << num << ' ';
        }else{
            for(int i = 0; i < cnt; i++){
                cout << '-';
            }
            cout << ' ';
        }
        
        num++;
    }


    return 0;
}