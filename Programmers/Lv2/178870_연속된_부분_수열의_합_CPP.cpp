#include <vector>
using namespace std;

vector<int> solution(vector<int> sequence, int k) {
    int n = sequence.size();
    int l = 0, r = 0;
    long long sum = 0;

    int bestL = 0, bestR = n - 1;
    bool found = false;

    while (true) {
        if (sum >= k) {
            if (sum == k) {
                if ((r - 1 - l < bestR - bestL)) {
                    bestL = l;
                    bestR = r - 1;
                    found = true;
                }
            }
            sum -= sequence[l++];
        } else { // sum < k
            if (r == n) break;
            sum += sequence[r++];
        }
    }

    return {bestL, bestR};
}
