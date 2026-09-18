#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    vector<int> arr;
    arr.resize(5);

    for(int i = 0; i < 5; i++){
        cin >> arr[i];
    }

    for(int i = 0; i < 5; i++){
        for(int j = 1; j < 5; j++){
            int temp;
            if(arr[j-1] > arr[j]){
                temp = arr[j-1];
                arr[j-1] = arr[j];
                arr[j] = temp;

                for(int k = 0; k < 5; k++){
                    cout << arr[k] << ' ';
                }
                cout << '\n';
            }
        }
    }

    return 0;
}   