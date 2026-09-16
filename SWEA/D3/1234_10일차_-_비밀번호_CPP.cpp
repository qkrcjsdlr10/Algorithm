#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 10;
    // cin >> tc;
    for(int t = 1; t <= tc; t++){
        int n = 0;
        string num = "";
        cin >> n >> num;
        while(1){
            bool check = false;
            for(int i = 0; i < num.size() - 1; i++){
                if(num[i] == num[i + 1]){
                    check = true;
                    for(int j = i; j < num.size() - 2; j++){
                        num[j] = num[j + 2];
                    }
                    num.resize(num.size() - 2);
                    // for(auto a : number){
                    //     cout << a;
                    // }
                    // cout << i;
                    // cout << '\n';
                    break;
                }
            }
            if(!check) break;
        }
        cout << '#' << t << ' ';
        for(auto a : num){
            cout << a;
        }
        cout << '\n';
    }
    
    // cout << num << '\n';
    // vector<int> number(n);

    // while(num > 0){
    //     int div = 0;
    //     div = num % 10;
    //     num /= 10;
    //     number[n - 1] = div;
    //     n--;
    //     cout << div;
    // }
    // n = number.size();


    

    return 0;
}