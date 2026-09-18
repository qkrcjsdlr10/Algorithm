#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);


    int money = 0;
    int bnp = 0;
    int timing = 0;
    int cnt = 0;
    int dday = 0;
    int b = 0;
    int arr[14] = {};
    int bmoney = 0;
    int asdf = 0;
    int ff = 0;

    cin >> money;
    bmoney = money;

    for(int i = 0; i < 14; i++){
        cin >> arr[i];
        if(i == 13){
            dday = arr[i];
        }
    }

    for(int i = 0; i < 13; i++){
        if(arr[i] <= money){
            int a = money / arr[i];
            ff += a;
            money = money % arr[i];
        }
    }

    for(int i = 1; i < 14; i++){
        // cout << cnt << ' ';

        if(arr[i-1] < arr[i]){
            if(cnt < 0){
                cnt = 0;
            }
            cnt++;
        }else if(arr[i] < arr[i-1]){
            if(cnt > 0){
                cnt = 0;
            }
            cnt--;
        }

        if(bmoney >= arr[i] && cnt <= -3){
            int a = bmoney / arr[i];
            asdf += a;

            bmoney %= arr[i];
        }else if(cnt >=3 && asdf > 0){
            bmoney += asdf * arr[i];
            asdf = 0;
        }
    }

    timing = bmoney + asdf * arr[13];

    bnp = money + ff * arr[13];

    // cout<< '\n' << bnp << ' ' << timing;

    if(bnp == timing){
        cout << "SAMESAME";
    }else if(bnp > timing){
        cout << "BNP";
    }else{
        cout << "TIMING";
    }

    return 0;
}