#include <bits/stdc++.h>
using namespace std;

int t, a, b;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> t;

    for(int i = 1; i <= t; i++){
        cin >> a >> b;
        if(a > b){
            cout << '#' << i << " >" << '\n';
        }else if(a < b){
            cout << '#' << i << " <" << '\n';
        }else{
            cout << '#' << i << " =" << '\n';
        }
    }
    
    return 0;
}
