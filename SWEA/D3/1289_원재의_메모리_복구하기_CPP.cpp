#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        string s = "";

        cin >> s;
        int res = 0;

        for(int i = 0; i < s.size(); i++){
            if(s[i] == '1'){
                for(int j = i; j < s.size(); j++){
                    if(s[j] == '0'){
                        s[j] = '1';
                    }else{
                        s[j] = '0';
                    }
                }
                res++;
            }
        }


        cout << '#' << t << ' ' << res << '\n';
    }
    

    return 0;
}