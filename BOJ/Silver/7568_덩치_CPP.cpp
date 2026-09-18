#include <bits/stdc++.h>
using namespace std;

int n, k;
int arr[50][2];
int sum[50];
int res[50];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;
    fill_n(res, 50, 1);

    for(int i = 0; i < n; i++){
        for(int j = 0; j < 2; j++){
            cin >> arr[i][j];
            sum[i] += arr[i][j];
        }
    }

    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < 2; j++){
    //         cout << arr[i][j] << ' ';
    //     }
    //     // cout << sum[i];
    //     cout << '\n';
    // }


    for(int i = 0; i < n; i++){
        k = 0;
        for(int j = 0; j < n; j++){
            if(arr[i][k] < arr[j][k] && arr[i][k+1] < arr[j][k+1]){
                res[i]++;
            }
        }
    }

    for(int i = 0; i < n; i++){
        cout << res[i] << ' ';
    }

    return 0;
}

