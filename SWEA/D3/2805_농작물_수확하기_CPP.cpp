#include <bits/stdc++.h>
using namespace std;

int arr[55][55];
int n = 0;
int sum;
int x = 0, y = 0;
int dx[] = {0};
int dy[] = {-1};

void func(int k, int step){
    if(step == 0){
        x = k;
        sum += arr[y][x];
        return;
    } 

    x = k;
    sum += arr[y][x];

    // cout << "arr[i][j]= " << arr[y][x] << ' '; 
    for(int i = k; i < step+k; i++){
        sum += arr[++y][++x];
        // cout << "arr[i][j]= " << arr[++y][++x] << ' '; 
        // y++;
        // x++;
    }

    for(int i = k; i < step+k; i++){
        sum += arr[++y][--x];
        // cout << "arr[i][j]= " << arr[++y][--x] << ' ';  
        // y++;
        // x--;
    }

    for(int i = k; i < step+k; i++){
        sum += arr[--y][--x];
        // cout << "arr[i][j]= " << arr[--y][--x] << ' '; 
        // y--;
        // x--;
    }

    for(int i = k; i < step+k-1; i++){
        sum += arr[--y][++x];
        // cout << "arr[i][j]= " << arr[--y][++x] << ' '; 
        // y--;
        // x--;
    }
    step--;
    func(k, step);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int a = 1; a <= tc; a++){
        cin >> n;
        string s[n];

        for(int i = 0; i < n; i++){
            cin >> s[i];
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                arr[i][j] = s[i][j] - '0';
            }
        }
        
        int start = n/2;
        int step = n/2;

        func(start, step);

        cout << '#' << a << ' ' << sum << '\n';
        y = 0;
        sum = 0;
    }
    

    return 0;
}