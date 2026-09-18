#include<bits/stdc++.h>
using namespace std;


vector<vector<int>> arr;
vector<bool> isused;
int n = 0;
int money = 0;
int mxmoney = 0;
void func(int day, int money){
    if(day >= n){
        mxmoney = max(money, mxmoney);
        return;
    }

    if(day + arr[day][0] <= n){
        func(day + arr[day][0], money + arr[day][1]);
    }
    
    func(day + 1, money);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    
    cin >> n;
    int cnt = 0;
    int day = 0;
    
    isused.resize(n);

    arr.resize(n, vector<int> (2));

    for(int i = 0; i < n; i++){
        cin >> arr[i][0] >> arr[i][1]; 
    }

    func(0, 0);

    cout << mxmoney;

    return 0;
}
//0  1  2  3  4  5  6  7  8  9 
//90 80 70 60 50 40 30 30  0  0