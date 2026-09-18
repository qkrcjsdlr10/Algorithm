#include <bits/stdc++.h>
using namespace std;

int num, n, d, idx, temp;
string s;
int arr[10], swapArr[10];
bool vis[1000001][11];
set<int> check[11];
int maxNum = -1;
int x;

void func(int n){
    s = "";
    for(int i = 0; i < idx; i++){
        s += to_string(arr[i]);
    }
    x = stoi(s);

    if(vis[x][n] == true) return;
    vis[x][n] = true;

    if(n == 0){
        if(x > maxNum){
            maxNum = x;
        }
        return;
    }
    
    for(int i = 0; i < idx - 1; i++){
        for(int j = i+1; j < idx; j++){
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;

            if (arr[0] != 0){
                func(n - 1);
            }

            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

void reverseArr(int start, int end){
    if(start >= end) return;
    temp = arr[start];
    arr[start] = arr[end];
    arr[end] = temp;

    reverseArr(start + 1, end -1);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    
    cin >> num >> n;

    idx = 0;
    maxNum = 0;
    x = 0;
    memset(vis, false, sizeof(vis));

    while(num > 0){
        arr[idx++] = num % 10;
        num /= 10;
    }
    // cout << idx;

    reverseArr(0, idx-1);
    
    if (idx == 1 || (idx == 2 && arr[1] == 0)) {
        cout << -1 << '\n';
        return 0;
    }
    func(n);

    cout << maxNum;
    
    return 0;
}