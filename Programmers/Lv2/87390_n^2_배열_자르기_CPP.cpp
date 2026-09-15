#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<int> solution(int n, long long left, long long right) {
    vector<int> answer;
    answer.reserve((size_t)(right - left + 1));

    for (long long k = left; k <= right; k++) {
        long long row = k / n;
        long long col = k % n;
        answer.push_back((int)(max(row, col) + 1));
    }
    return answer;
}