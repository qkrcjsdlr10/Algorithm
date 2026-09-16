#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int testCase = 0;
    cin >> testCase;

    for(int t = 1; t <= testCase; t++){
        int n = 0;
        cin >> n;
        int num = 0;
        int maxPrice = 0, nowPrice = 0;
        long long sum = 0;

        stack<int> s;

        for(int i = 0; i < n; i++){
            cin >> num;
            s.push(num);
        }

        while(!s.empty()){
            nowPrice = s.top(); 
            s.pop();
            if(nowPrice > maxPrice){
                maxPrice = nowPrice;
                continue;
            }else{
                sum += maxPrice - nowPrice;
            }
        }

        cout << '#' << t << ' ' << sum << '\n';
    }

    return 0;
}