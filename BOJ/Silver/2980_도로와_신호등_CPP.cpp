#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int l = 0;
    int time = 0;
    int pos = 0;
    vector<vector<int>> arr;

    cin >> n >> l;
    arr.resize(n, vector<int>(5));

    for (int i = 0; i < n; i++) {
        cin >> arr[i][0] >> arr[i][1] >> arr[i][2];
        arr[i][3] = 0; 
        arr[i][4] = 0; 
    }

    int idx = 0;

    while (pos < l) {
        time++;
        pos++;

        for (int i = 0; i < n; i++) {
            arr[i][4]++;
            if (arr[i][3] % 2 == 0 && arr[i][1] == arr[i][4]) {
                arr[i][3]++;
                arr[i][4] = 0;
            } else if (arr[i][3] % 2 == 1 && arr[i][2] == arr[i][4]) {
                arr[i][3]++;
                arr[i][4] = 0;
            }
        }
        if (idx < n && pos == arr[idx][0]) {
            if (arr[idx][3] % 2 == 0) {
                int remain = arr[idx][1] - arr[idx][4];

                for (int t = 0; t < remain; t++) {
                    time++;
                    for (int i = 0; i < n; i++) {
                        arr[i][4]++;
                        if (arr[i][3] % 2 == 0 && arr[i][1] == arr[i][4]) {
                            arr[i][3]++;
                            arr[i][4] = 0;
                        } else if (arr[i][3] % 2 == 1 && arr[i][2] == arr[i][4]) {
                            arr[i][3]++;
                            arr[i][4] = 0;
                        }
                    }
                }
            }
            idx++;
        }
    }

    cout << time;
    return 0;
}
