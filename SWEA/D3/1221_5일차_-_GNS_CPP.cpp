#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc;
    cin >> tc;

    vector<string> str = {
        "ZRO", "ONE", "TWO", "THR", "FOR",
        "FIV", "SIX", "SVN", "EGT", "NIN"
    };

    for(int a = 0; a < tc; a++){
        string t;
        int total;

        cin >> t >> total;

        vector<int> arr(10, 0);

        for(int i = 0; i < total; i++){
            string temp;
            cin >> temp;

            auto it = find(str.begin(), str.end(), temp);
            int idx = it - str.begin();

            arr[idx]++;
        }

        cout << t << '\n';

        for(int i = 0; i < 10; i++){
            while(arr[i]--){
                cout << str[i] << ' ';
            }
        }

        cout << '\n';
    }

    return 0;
}