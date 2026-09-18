#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    int n = 0;

    cin >> n;
    
    deque<int> res;

    res.push_front(n);

    for(int i = n-1; i > 0; i--){
        res.push_front(i);

        for(int j = 0; j < i; j++){
            res.push_front(res.back());
            res.pop_back();
        }
    }

    for(int i = 0; i < n; i++){
        cout << res[i] << ' ';
    }
    cout << '\n';

    return 0;
}