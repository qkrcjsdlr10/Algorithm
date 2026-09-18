#include <bits/stdc++.h>
using namespace std;

int n;
int d;
int nd;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int nx;
int ny;
int num;
int dir;

void func(vector<vector<int>>& arr, int x, int y, int dir, int step, int& num){
    if(num > n * n) return;

    for(int i = 0; i < step; i++) {
        x += dx[dir];
        y += dy[dir];
        if(x >= 0 && x < n && y >= 0 && y < n && arr[y][x] == 0) {
            arr[y][x] = num++;
        }
    }

    dir = (dir + 1) % 4;

    func(arr, x, y, dir, step + (dir % 2 == 0 ? 1 : 0), num);
}

void fillLayer(vector<vector<int>>& arr, int x, int y, int dir, int step, int& num, int size) {
    if (num > size * size) return; // 모든 숫자를 채웠으면 종료

    // 현재 방향으로 step만큼 이동
    for(int i = 0; i < step; i++) {
        x += dx[dir];
        y += dy[dir];
        if(x >= 0 && x < size && y >= 0 && y < size && arr[y][x] == 0) {
            arr[y][x] = num++;
        }
    }

    // 방향 전환 (상 → 우 → 하 → 좌)
    dir = (dir + 1) % 4;

    // 다음 층 재귀 호출 (step 증가) 상, 하 방향일때 스탭 증가가
    fillLayer(arr, x, y, dir, step + (dir % 2 == 0 ? 1 : 0), num, size);
}

void print(vector<vector<int>>& arr){
    cout << '\n';
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            cout << arr[i][j] << " ";
        }
        cout << '\n';
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;

    vector<vector<int>> arr (n, vector<int>(n, 0));

    int startNum = n/2;
    num = 1;
    arr[startNum][startNum] = num++;

    func(arr, startNum, startNum, 0, 1, num);

    // // cout << startNum;
    print(arr);

    return 0;
}
