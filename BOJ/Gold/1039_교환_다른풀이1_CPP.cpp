#include <bits/stdc++.h>
using namespace std;

int maxNum = -1;
set<pair<string, int>> visited; // (현재 숫자 문자열, 남은 교환 횟수) 쌍 저장

void func(string s, int remain) {
    // 이미 방문한 상태인 경우 종료
    if(visited.find({s, remain}) != visited.end()) return;
    visited.insert({s, remain});

    // 교환 횟수 소진 시 최대값 갱신
    if(remain == 0) {
        maxNum = max(maxNum, stoi(s));
        return;
    }

    // 모든 가능한 위치 교환
    for(int i = 0; i < s.size()-1; i++) {
        for(int j = i+1; j < s.size(); j++) {
            // 0 시작 방지
            if(i == 0 && s[j] == '0') continue;
            
            swap(s[i], s[j]); // 교환
            func(s, remain-1);
            swap(s[i], s[j]); // 원복
        }
    }
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    string N;
    int K;
    cin >> N >> K;

    // 예외 처리: 한 자리 수 또는 2자리인데 0 포함
    if(N.size() == 1 || (N.size() == 2 && N[1] == '0')) {
        cout << -1;
        return 0;
    }

    func(N, K);
    cout << (maxNum == -1 ? -1 : maxNum);

    return 0;
}
