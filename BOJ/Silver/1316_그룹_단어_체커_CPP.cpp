#include <bits/stdc++.h>
using namespace std;



int n;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int cnt = 0;
    int arr[30] = {};
    bool a = false;

    cin >> n;
    vector<string> s(n);
    // string s = "";
    char c;

    for(int i = 0; i < n; i++){
        cin >> s[i];
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < s[i].size(); j++){
            arr[s[i][j]-'a'] = 0;
        }
    }

    // for(int i = 0; i < n; i++){
    //     for(int j = 0; j < s[i].size(); j++){
    //         cout << arr[s[i][j]] << ' ';
    //     }
    // }

    for(int i = 0; i < n; i++){
        a = true;
        for(int j = 0; j < s[i].size(); j++){  
            if(a){
                c = s[i][j];
                if(arr[s[i][j]-'a'] != 0 && c != s[i][j-1] && j > 0){
                    // cout << "c= " <<c << "s= " << s[i][j-1] << "arr= " << arr[s[i][j]-'a'] << '\n';
                    cnt++;
                    a = false;
                    continue;
                }
                arr[s[i][j]-'a']++;
            }
        }
        
        for(int x = 0; x < 30; x++){
            arr[x] = 0;
        }
    }

    cout << n-cnt;

    return 0;
}