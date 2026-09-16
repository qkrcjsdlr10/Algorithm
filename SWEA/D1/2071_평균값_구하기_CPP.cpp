#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 1; t <= tc; t++){
        double sum = 0;

        for(int i = 0; i < 10; i++){
            int a = 0;
            cin >> a;
            sum += a;
        }
    
        int a =  sum;
        int res = 0;
        res = sum / 10;
    
        if(a % 10 >= 5){
            res += 1;
        }
    
        cout << '#' << t << ' ' << res << '\n';
    }

    return 0;
}