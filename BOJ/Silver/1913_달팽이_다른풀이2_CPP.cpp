#include <iostream>
#include <vector>

using namespace std;

// 방향 벡터: 상, 우, 하, 좌
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

// 재귀적으로 나선형 배열 채우기
void fillLayer(vector<vector<int>>& arr, int x, int y, int dir, int step, int& num, int size) {
    if (num > size * size) return; // 모든 숫자를 채웠으면 종료

    // 현재 방향으로 step만큼 이동
    for (int i = 0; i < step; i++) {
        x += dx[dir];
        y += dy[dir];
        if (x >= 0 && x < size && y >= 0 && y < size && arr[y][x] == 0) {
            arr[y][x] = num++;
        }
    }

    // 방향 전환 (상 → 우 → 하 → 좌)
    dir = (dir + 1) % 4;

    // 다음 층 재귀 호출 (step 증가) 상, 하 방향일때 스탭 증가가
    fillLayer(arr, x, y, dir, step + (dir % 2 == 0 ? 1 : 0), num, size);
}

void generateSpiral(vector<vector<int>>& arr, int size) {
    int centerX = size / 2;
    int centerY = size / 2;
    int num = 1;
    arr[centerY][centerX] = num++; // 중심값 설정

    fillLayer(arr, centerX, centerY, 0, 1, num, size); // 초기 방향: 우, step:1
}

void printMatrix(const vector<vector<int>>& arr) {
    for (const auto& row : arr) {
        for (int num : row) {
            cout << num << "\t";
        }
        cout << endl;
    }
}

int main() {
    int n;
    cout << "Enter the size of the matrix: ";
    cin >> n;

    vector<vector<int>> arr(n, vector<int>(n, 0));

    generateSpiral(arr, n);
    printMatrix(arr);

    return 0;
}
