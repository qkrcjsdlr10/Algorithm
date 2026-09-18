#include <bits/stdc++.h>
using namespace std;

int n;
int arr[100][100];

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;
    int arr1[n][2];
    int cnt = 0;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < 2; j++){
            cin >> arr1[i][j];
        }
    }

    for(int i = 0; i < n; i++){
        int m = 0;
        int y = arr1[i][m];
        int x = 100-arr1[i][m+1];

        for(int k = x; k > x - 10; k--){
            for(int l = y; l < y + 10; l++){
                arr[k][l] = 1;
            }
        }
    }

    for(int i = 0; i < 100; i++){
        for(int j = 0; j < 100; j++){
            // cout << arr[i][j];
            if(arr[i][j] == 1){
                cnt++;
            }
        }
        // cout << '\n';
    }

    cout << cnt;
    

    return 0;
}

