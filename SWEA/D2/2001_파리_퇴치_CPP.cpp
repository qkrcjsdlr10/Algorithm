#include <bits/stdc++.h>
using namespace std;

int n, m;
int sum, maxSum;
int arr[15][15];

void func(){
    for(int i = 0; i <= n - m; i++){
        for(int j = 0; j <= n - m; j++){
            for(int k = i; k < i + m; k++){
                for(int l = j; l < j + m; l++){
                    // cout << k << l << ' ';
                    sum += arr[k][l];
                }
                
                // cout << '\n';
                
            }
            // cout << sum << '\n';
            if(sum > maxSum){
                maxSum = sum;
            }
            sum = 0;
            // cout << '\n';
        }
    }

}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 0;

    cin >> t;
    for(int a = 1; a <= t; a++){
        cin >> n >> m;

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> arr[i][j];
            }
        }
        sum = 0;
        maxSum = 0;

        func();

        cout << '#' << a << ' ' << maxSum << '\n';
    }

    

    return 0;
}