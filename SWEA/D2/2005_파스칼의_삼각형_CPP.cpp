#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int testCase = 0;
    cin >> testCase;

    for(int t = 0; t < testCase; t++){
        int tc = 0;

        int n = 0;
        cin >> n;
    
        vector<vector<int>> arr;
        arr.resize(n+1, vector<int>(n+1, 1));
    
        for(int i  = 0; i < n; i++){
            for(int j = n-1; j >= i+1; j--){
                arr[i][j] = 0;
            }
        }
    
        for(int i  = 1; i < n; i++){
            for(int j = 1; j < i; j++){
                arr[i][j] = arr[i-1][j-1] + arr[i-1][j];
            }
        }

        cout << '#' << t+1 << '\n';
    
        for(int i  = 0; i < n; i++){
            for(int j = 0; j < i+1; j++){
                cout << arr[i][j] << ' ';
            }
            cout << '\n';
        }
    }

    return 0;
}