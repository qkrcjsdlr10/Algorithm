#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> money) {
    int n = money.size();
    if (n == 1) return money[0];

    // Case 1: 첫 번째 집을 털고, 마지막 집을 털지 않는 경우 (인덱스 0 ~ n-2)
    vector<int> dp1(n);
    dp1[0] = money[0];
    dp1[1] = money[0]; // 첫 집을 털었으므로 두 번째 집은 못 틂
    for (int i = 2; i < n - 1; i++) {
        dp1[i] = max(dp1[i - 1], dp1[i - 2] + money[i]);
    }
    int case1 = dp1[n - 2];

    // Case 2: 첫 번째 집을 털지 않는 경우 (인덱스 1 ~ n-1)
    vector<int> dp2(n);
    dp2[0] = 0;
    dp2[1] = money[1];
    for (int i = 2; i < n; i++) {
        dp2[i] = max(dp2[i - 1], dp2[i - 2] + money[i]);
    }
    int case2 = dp2[n - 1];

    return max(case1, case2);
}