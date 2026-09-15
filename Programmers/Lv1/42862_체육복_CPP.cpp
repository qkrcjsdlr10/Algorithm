#include <bits/stdc++.h>
using namespace std;

int solution(int n, vector<int> lost, vector<int> reserve) {
    sort(lost.begin(), lost.end());
    sort(reserve.begin(), reserve.end());

    // 1) 겹치는 학생 먼저 제거
    for (int i = 0; i < (int)lost.size(); ) {
        auto it = find(reserve.begin(), reserve.end(), lost[i]);
        if (it != reserve.end()) {
            reserve.erase(it);
            lost.erase(lost.begin() + i);
        } else {
            i++;
        }
    }

    // 2) 빌리기
    int cnt = 0;
    for (int x : lost) {
        auto itL = find(reserve.begin(), reserve.end(), x - 1);
        if (itL != reserve.end()) {
            reserve.erase(itL);
            cnt++;
            continue;
        }
        auto itR = find(reserve.begin(), reserve.end(), x + 1);
        if (itR != reserve.end()) {
            reserve.erase(itR);
            cnt++;
        }
    }

    return n - (int)lost.size() + cnt;
}