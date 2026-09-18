#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int res = 0;
    string s = "";
    string str = "";

    getline(cin, s);
    getline(cin, str);
    int i = 0;

    if(s.size() < str.size()){
        cout << 0;
        return 0;
    }

    while(i <= s.size() - str.size()){
        int cnt = 0;
        for(int j = 0; j < str.size(); j++){
            if(s[i+j] == str[j]){
                cnt++;
            }
        }
        if(cnt == str.size()){
            res++;
            i += cnt;
        }else{
            i++;
        }
    }

    cout << res;
    
    return 0;
}
