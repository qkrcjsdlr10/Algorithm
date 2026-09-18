#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    char arr[5][15] = {};
    string res = "";

    for(int i = 0; i < 5; i++){
        string s = "";
        cin >> s;
        for(int j = 0; j < s.size(); j++){
            arr[i][j] = s[j];
        }
    }

    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 5; j++){
            if(arr[j][i]){
                res += arr[j][i];
            }
            
        }
    }

    cout << res;

    return 0;
}