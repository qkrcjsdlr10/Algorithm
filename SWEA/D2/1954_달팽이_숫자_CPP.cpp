#include <bits/stdc++.h>
using namespace std;

int arr[11][11];

void func(int size, int num, int x, int y){

    if(size == 1){
        arr[y][x] = num;
        return;
    }else if(size == 0){
        return;
    } 

    for(int i = 0; i < size; i++){
        arr[y][x] = num++;
        x++;
    }
    x--;
    y++;

    size--;

    for(int i = 0; i < size; i++){
        arr[y][x] = num++;
        y++;
    }
    y--;
    x--;

    for(int i = 0; i < size; i++){
        arr[y][x] = num++;
        x--;
    }
    y--;
    x++;
    size--;

    for(int i = 0; i < size; i++){
        arr[y][x] = num++;
        y--;
    }
    y++;
    x++;
    func(size, num, x, y);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int testCase = 0;
    cin >> testCase;

    for(int t = 0; t < testCase; t++){
        int n = 0;
        cin >> n;
        int num = 1;

        func(n, num, 0, 0);

        cout << '#' << t+1 << '\n';
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cout << arr[i][j] << ' ';
            }
            cout << '\n';
        }
    }

    return 0;
}