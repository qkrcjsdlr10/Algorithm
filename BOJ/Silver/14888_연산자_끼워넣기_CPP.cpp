#include <bits/stdc++.h>
using namespace std;

int limit = 0;
int mx = -1e9;
int mn = 1e9;
vector<int> op(4);
vector<int> num;

void func(int k, int res, int idx){
    if(k == limit){
        mx = max(mx, res);
        mn = min(mn, res);
        return;
    }

    if(op[0] > 0){
        op[0]--;
        func(k+1, res + num[idx], idx+1);
        op[0]++;
    }

    if(op[1] > 0){
        op[1]--;
        func(k+1, res - num[idx], idx+1);
        op[1]++;
    }

    if(op[2] > 0){
        op[2]--;
        func(k+1, res * num[idx], idx+1);
        op[2]++;
    }

    if(op[3] > 0){
        if(num[idx] != 0){
            op[3]--;
            func(k+1, res / num[idx], idx+1);
            op[3]++;
        }
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    
    
    cin >> n;
    num.resize(n);
    

    for(int i = 0; i < n; i++){
        cin >> num[i];
    }

    for(int i = 0; i < 4; i++){
        cin >> op[i];
        limit += op[i];
    }

    func(0, num[0], 1);
    // cout << limit;
    cout << mx << '\n' << mn;

    return 0;
}