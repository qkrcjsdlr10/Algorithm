#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    for(int t = 0; t < 10; t++){
        int tc = 0;
        cin >> tc;

        string target = "";
        string s = "";
        cin >> target >> s;
        
        // cin >> s;
        int cnt = 0;
    
        for(int i = 0; i <= s.size()-target.size(); i++){
            string str = "";
            for(int k = i; k < i+target.size(); k++){
                str += s[k];
            }
            if(str == target){
                cnt++;
            }
        }
    
        cout << '#' << tc << ' ' << cnt << '\n';

    }

    return 0;
}