#include <bits/stdc++.h>
using namespace std;

int arr[10];
int t;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int max = 0;

    cin >> t;

    for(int i = 1; i <= t; i ++){
        for(int i = 0; i < sizeof(arr)  / sizeof(arr[0]); i ++){
            cin >> arr[i];
        }

        for(int i = 0; i < sizeof(arr) / sizeof(arr[0]); i ++){
            if(max < arr[i]){
            max = arr[i]; 
            }
        }
        
        cout << '#' << i << ' ' << max << '\n';
        max = 0;
    }

    
    
    return 0;
}
