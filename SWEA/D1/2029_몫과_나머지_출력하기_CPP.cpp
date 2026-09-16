#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int i = 1; i <= tc; i++){
        int num = 0;
        int div = 0;
        cin >> num >> div;
    
        cout << '#' << i << ' ' << num / div << ' ' << num % div << '\n';
    }



    return 0;
}