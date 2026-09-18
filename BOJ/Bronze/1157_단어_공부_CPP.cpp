#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int arr[26] = {};

    char res = ' ';
    string s = "";
    cin >> s;

    for(auto c : s){
        int a = 0;
        if(c >= 'a' && c <= 'z'){
            a = c - 'a';
        }else if(c >= 'A' && c <= 'Z'){
            a = c - 'A';
        }

        arr[a]++;
    }

    int idx = 0;
    int mxIdx = 0;

    for(int i = 0; i < 26; i++){
        if(idx < arr[i]){
            idx = arr[i];
            mxIdx = i;
        }
    }
    int b = 0;
    for(int i = 0; i < 26; i++){
        if(idx == arr[i]){
            b++;
        }
    }
    if(b > 1){
        cout << '?';
        return 0;
    }

    res = 'A' + mxIdx;

    cout << res;

    return 0;
}