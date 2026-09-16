#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    vector<int> arr;
    int size = 0;
    cin >> size;

    arr.resize(size);

    for(int i = 0; i < size; i++){
        cin >> arr[i];
    }

    for(int i = 0; i < size-1; i++){
        for(int j = i+1; j < size; j++){
            if(arr[i] > arr[j]){
                int temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;  
            }
        }
    }

    cout << arr[size/2];

    return 0;
}