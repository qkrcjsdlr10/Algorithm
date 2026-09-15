#include <string>
#include <vector>

using namespace std;
int res;

void func(int start, int cnt, int sum, vector<int>& number, int& res) {
    if (cnt == 3) {
        if (sum == 0) res++;
        return;
    }
    for (int i = start; i < number.size(); i++) {
        func(i + 1, cnt + 1, sum + number[i], number, res);
    }
}

int solution(vector<int> number) {
    int answer = 0;
    
    func(0, 0, 0, number, res);
    answer = res;
    
    return answer;
}