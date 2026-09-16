#include <bits/stdc++.h>
using namespace std;

int n;
int arr[10][10];
int rotarr[10][10];

vector<vector<int>> rot(vector<vector<int>> arr){
    int idxX = 0;
    int idxY = 0;
    vector<vector<int>> rotArr(n, vector<int>(n));

    for(int i = 0; i < n; i++){
        for(int j = n-1; j >= 0; j--){
            rotArr[idxX][idxY] = arr[j][i];
            idxY++;
        }
        idxY = 0;
        idxX++;
    }

    return rotArr;
}

vector<vector<int>> func(vector<vector<int>> arr){
    int idxX = n-1;
    int idxY = 0;
    vector<vector<int>> rotArr(n, vector<int>(n));

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            rotArr[idxY][idxX] = arr[i][j];
            idxY++;
        }
        idxY = 0;
        idxX--;
    }

    return rotArr;
}

int main(){
    int t = 0;
    cin >> t;

    

    for(int x = 0; x < t; x++){
        cin >> n;
        vector<vector<int>> arr(n, vector<int>(n));

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> arr[i][j];
            }
        }

        auto rot1 = func(arr);
        auto rot2 = func(rot1);
        auto rot3 = func(rot2);

        cout << '#' << x+1 << '\n';

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cout << rot1[i][j];
            }
            cout << ' ';

            for(int j = 0; j < n; j++){
                cout << rot2[i][j];
            }
            cout << ' ';

            for(int j = 0; j < n; j++){
                cout << rot3[i][j];
            }

            cout << '\n';
        }
    }

    
}