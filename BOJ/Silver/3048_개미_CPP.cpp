#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int a = 0;
    int b = 0;
    int c = 0;


    cin >> a >> b;

    vector<vector<char>> arr1;
    vector<vector<char>> arr2;

    vector<vector<char>> arr;
    arr.resize(a+b, vector<char> (2));

    arr1.resize(a, vector<char> (2));
    arr2.resize(b, vector<char> (2));

    for(int i = a-1; i >= 0; i--){
        cin >> arr1[i][0];
        arr1[i][1] = '1';
    }

    for(int i = 0; i < b; i++){
        cin >> arr2[i][0];
        arr2[i][1] = '2';
    }

    cin>> c;

    for(int i = 0; i < a+b; i++){
        if(i < a){
            arr[i][0] = arr1[i][0];
            arr[i][1] = arr1[i][1];
        }else{
            arr[i][0] = arr2[i-a][0];
            arr[i][1] = arr2[i-a][1];
        }
    }

    // for(int i = 0; i < a+b; i++){
    //     cout << arr[i][0] << ' ';
    // }
    
    // cout << '\n';

    // for(int i = 0; i < a+b; i++){
    //     cout << arr[i][1] << ' ';
    // }


    for(int t = 0; t < min(c, a+b); t++){
        for(int i = 0; i < a+b-1; i++){
            if(arr[i][1] == '1' && arr[i+1][1] == '2'){
                char temp = arr[i][1];
                arr[i][1] = arr[i+1][1];
                arr[i+1][1] = temp;
                // cout <<"aasdfasdfasdfasdf";
                char temp1 = arr[i][0];
                arr[i][0] = arr[i+1][0];
                arr[i+1][0] = temp1;
                i++;
            }
        }
    }
    // cout << '\n';
    for(int i = 0; i < a+b; i++){
        cout << arr[i][0];
    }
    
    // cout << '\n';

    // for(int i = 0; i < a+b; i++){
    //     cout << arr[i][1];
    // }
    

    return 0;
}