#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(i == j){
                cout << '#';
            }else{
                cout << '+';
            }
        }
        cout << '\n';
    }
    // cout << '\n';

    // for(int i = 0; i < 5; i++){
    //     for(int j = 0; j < 5; j++){
    //         if(i + j == 4){
    //             cout << '#';
    //         }else{
    //             cout << '+';
    //         }
    //     }
    //     cout << '\n';
    // }

    return 0;
}