#include <string>
#include <vector>

using namespace std;

vector<vector<int>> res;

void hanoi(int n, int from, int temp, int to) {
    if (n == 1) {
        res.push_back({from, to});
        return;
    }

    // 1. 위 n-1개를 보조 기둥으로 옮김
    hanoi(n - 1, from, to, temp);

    // 2. 가장 큰 원판을 목표 기둥으로 옮김
    res.push_back({from, to});

    // 3. 보조 기둥에 있던 n-1개를 목표 기둥으로 옮김
    hanoi(n - 1, temp, from, to);
}

vector<vector<int>> solution(int n) {
    vector<vector<int>> answer;
    
    hanoi(n, 1, 2, 3);
    
    answer = res;
    
    return answer;
}