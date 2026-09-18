#include <bits/stdc++.h>
using namespace std;

int n, m, k;
int num[10];
int arr[10];
bool isused[10];
int res = 0;

void func(int k){
    if(k == m){
        int sum = 0;
        for(int i = 0; i < m; i++){
            cout << arr[i] << ' ';
            sum += arr[i];
        }
        if(sum == 0) res++;
        cout << '\n';
        return;
    }

    for(int i = 0; i < n; i++){
        if(!isused[i]){
            arr[k] = num[i];
            isused[i] = 1;
            func(k + 1);
            isused[i] = 0;
        }
    }

}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    
    cin >> n >> m;

    for(int i = 0; i < n; i++){
        cin >> num[i];
    }

    sort(num, num + n);

    func(0);

    cout << res;

    return 0;
}