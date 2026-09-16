#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0, sum = 0, t = 0;

    cin >> t;

    for(int i = 1; i <= t; i++){

        cin >> n;

        for(int i = 1; i <= n; i++){
            if(i%2 != 0){
                sum += i;
            }else{
                sum -= i;
            }
        }

        cout <<"#"<< i << " " << sum << "\n";
        sum = 0;
    }

    

    return 0;
}