#include<bits/stdc++.h>
using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int n;
int x, y, d;
int nx, ny, nd;
int num;
int step, line;
int searchNum, sX, sY;

// int dx[] = {0, 1, 0, -1}; // 상, 우, 하, 좌 (y 좌표 변화)
// int dy[] = {-1, 0, 1, 0}; // 상, 우, 하, 좌 (x 좌표 변화)

void snail(vector<vector<int>>& arr){
    if(num > n*n){
        return;
    }

    arr[y][x] = num;
    
    if(searchNum == num){
        sX = x;
        sY = y;
    }
    num++;
    y--;
    arr[y][x] = num;
    
    
    for(int i = 0; i < step; i++){
        if(searchNum == num){
            sX = x;
            sY = y;
        }
        num++;
        x+=1;
        arr[y][x] = num;
    }

    step++;

    for(int i = 0; i < step; i++){
        if(searchNum == num){
            sX = x;
            sY = y;
        }
        num++;
        y+=1;
        arr[y][x] = num;
    }

    for(int i = 0; i < step; i++){
        if(searchNum == num){
            sX = x;
            sY = y;
        }
        num++;
        x-=1;
        arr[y][x] = num;
    }

    // cout << "x= " << x << "y= " << y << '\n';
    // cout << "step= " << step << '\n';

    for(int i = 0; i < step; i++){
        if(searchNum == num){
            sX = x;
            sY = y;
        }
        num++;
        y-=1;
        arr[y][x] = num;
    }

    if(num >= n*n){
        return;
    }else{
        step++;
        snail(arr);
    }
}

void print(vector<vector<int>>& a){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << a[i][j] << " ";
        }
        cout << '\n';
    }

    cout << sY+1 << ' ' << sX+1;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    num = 1;
    step = 1;
    line = 1;

    
    cin >> n >> searchNum;
    vector<vector<int>> arr (n, vector<int>(n, 0));
    int start = n/2;
    x = start;
    y = start;
    // 

    snail(arr);
    print(arr);
    

    return 0;
}