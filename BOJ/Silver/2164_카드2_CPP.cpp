#include <bits/stdc++.h>
using namespace std;

void func(){
    
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    cin >> n;
    
    deque<int> arr;

    for(int i = 1; i <= n; i++){
        arr.push_back(i);
    }

    while(arr.size() > 1){
        arr.pop_front();
        arr.push_back(arr.front());
        arr.pop_front();
    }

    cout << arr.front();

    return 0;
}