#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int m = 0;


    cin >> n >> m;

    vector<vector<int>> arr;
    arr.resize(n, vector<int> (m));

    for(int i = 0; i < n; i++){
        string str = "";
        cin >> str;
        for(int j = 0; j < m; j++){
            arr[i][j] = str[j] - '0';
        }
    }
    bool check = false;
    int size = min(n, m);
    int result = 0;
    while(size > 1 && check == false){
        for(int i = 0; i < n; i++){
            if(check == true) break;
            for(int j = 0; j < m; j++){
                if(i + size > n || j + size > m){
                    break;
                }
    
                if(arr[i][j] == arr[i][j+size-1] &&
                    arr[i][j] == arr[i+size-1][j]&& 
                    arr[i][j] == arr[i+size-1][j+size-1]){
                    check = true;
                    break;
                }
            }
            
        }
        if(check == false){
            size--;
        }else{
            break;
        }
        
    }

    cout << size*size;


    return 0;
}