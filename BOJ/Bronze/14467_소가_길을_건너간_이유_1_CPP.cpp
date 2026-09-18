#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    cin >> n;
    int res = 0;

    vector<vector<int>> arr;
    vector<vector<int>> arr2;
    vector<stack<pair<int, int>>> arr3;
    arr.resize(n, vector<int> (2));
    arr2.resize(10, vector<int> (2));

    arr3.resize(10);

    for(int i = 0; i < n; i++){
        cin >> arr[i][0] >> arr[i][1];
    }

    for(int i = 0; i < n; i++){
        // if(arr[i][1] == 0){
        //     arr2[arr[i][0]-1][0]++;
        // }else{
        //     arr2[arr[i][0]-1][1]++;
        // }
        if(arr3[arr[i][0]-1].empty()){
            arr3[arr[i][0]-1].push({arr[i][0], arr[i][1]});
        }
        else{
            if(arr3[arr[i][0]-1].top().first == arr[i][0] && arr3[arr[i][0]-1].top().second != arr[i][1]){
                arr3[arr[i][0]-1].push({arr[i][0], arr[i][1]});
            }
        }
    }
    // cout << '\n';

    for(int i = 0; i < 10; i++){
        // cout << arr3[i].size() << ' ';
        if(arr3[i].size() > 1){
            res += arr3[i].size() - 1;
        }
    }

    cout << res;

    return 0;
}