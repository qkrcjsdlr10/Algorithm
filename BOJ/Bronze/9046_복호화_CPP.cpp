#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;
    cin.ignore();

    for(int i = 0; i < tc; i++){
        int arr[27] = {0};
        int idx = 0;
        int b = 0;
        int d = 0;
    
        string str = "wghuvijxpqrstacdebfklmnoyz";
    
        string s = "";
        getline(cin, s);
    
        for(int i = 0; i < s.size(); i++){
            if(s[i] == ' ') continue;
            int a = int(s[i]) - 'a';
            // cout << a << '\n';
            arr[a]++;
        }
    
        for(int i = 0; i < 27; i++){
            if(idx < arr[i]){
                idx = arr[i];
                d = i;
            }
        }
        // cout << idx << ' ';
    
        for(int i = 0; i < 27; i++){
            // cout << arr[i] << ' ';
            if(idx == arr[i]){
                b++;
            }
        }
    
        if(b > 1){
            cout << '?' << '\n';
            continue;
        }
    
        // cout << d << ' ';
    
        char c = 'a' + d;
    
        cout << c << '\n';
    }



    return 0;
}