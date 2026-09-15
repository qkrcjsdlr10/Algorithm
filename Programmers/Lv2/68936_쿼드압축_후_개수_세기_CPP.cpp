#include <vector>
using namespace std;

void quad(int y, int x, int size, const vector<vector<int>>& a, vector<int>& ans) {
    int v = a[y][x];
    bool same = true;

    for (int i = y; i < y + size && same; i++) {
        for (int j = x; j < x + size; j++) {
            if (a[i][j] != v) { same = false; break; }
        }
    }

    if (same) {                 // 압축 성공
        ans[v]++;
        return;
    }

    int m = size / 2;           // 압축 실패 -> 4분할
    quad(y, x, m, a, ans);
    quad(y, x + m, m, a, ans);
    quad(y + m, x, m, a, ans);
    quad(y + m, x + m, m, a, ans);
}

vector<int> solution(vector<vector<int>> arr) {
    vector<int> ans(2, 0);
    quad(0, 0, (int)arr.size(), arr, ans);
    return ans;
}
