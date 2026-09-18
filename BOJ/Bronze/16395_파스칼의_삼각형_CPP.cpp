#include <bits/stdc++.h>

using namespace std;

vector<vector<int>> arr;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);


    int n = 0;
    int m = 0;
    cin >> n >> m;

    vector<vector<int>> arr(n, vector<int>(n));

    fill(arr.begin(), arr.end(), vector<int> (n, 1));

    for(int i = 0; i < n; i++){
        for(int j = 0; j <= i; j++){
            if(j != 0 && j != i){
                arr[i][j] = arr[i-1][j-1] + arr[i-1][j];
            }
        }
    }
    
    cout << arr[n-1][m-1];
    

    return 0;
}