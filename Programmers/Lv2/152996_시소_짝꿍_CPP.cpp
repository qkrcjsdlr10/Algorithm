#include <bits/stdc++.h>
using namespace std;

long long solution(vector<int> weights) {
    sort(weights.begin(), weights.end());

    unordered_map<long long, long long> cnt;
    cnt.reserve(weights.size() * 2);

    long long answer = 0;

    for (long long w : weights) {
        // 같은 무게 (1:1)
        answer += cnt[w];

        // 비율 케이스들 (거리 2,3,4 조합)
        if ((w * 2) % 3 == 0) answer += cnt[w * 2 / 3]; // 3:2 (w가 더 큼일 때)
        if ((w * 3) % 2 == 0) answer += cnt[w * 3 / 2]; // 2:3
        if (w % 2 == 0)       answer += cnt[w / 2];     // 2:4 (=1:2)
        if ((w * 3) % 4 == 0) answer += cnt[w * 3 / 4]; // 4:3
        if ((w * 4) % 3 == 0) answer += cnt[w * 4 / 3]; // 3:4

        cnt[w]++;
    }

    return answer;
}
