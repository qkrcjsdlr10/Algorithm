#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool canPlace(const vector<vector<string>>& park, int x, int y, int len) {
    int n = park.size();
    int m = park[0].size();

    if (x + len > n || y + len > m) return false;

    for (int i = x; i < x + len; i++) {
        for (int j = y; j < y + len; j++) {
            if (park[i][j] != "-1") return false;
        }
    }

    return true;
}

int solution(vector<int> mats, vector<vector<string>> park) {
    int n = park.size();
    int m = park[0].size();
    int mx = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (park[i][j] != "-1") continue;

            for (int len = 1; ; len++) {
                if (!canPlace(park, i, j, len)) break;
                mx = max(mx, len);
            }
        }
    }

    sort(mats.rbegin(), mats.rend());

    for (int mat : mats) {
        if (mat <= mx) return mat;
    }

    return -1;
}