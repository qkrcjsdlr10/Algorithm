#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int n) {
    string ternary = "";

    // 1. 3진법 변환
    while (n > 0) {
        ternary += to_string(n % 3);
        n /= 3;
    }
    
    int answer = 0;
    int base = 1;
    for (int i = ternary.size() - 1; i >= 0; i--) {
        answer += (ternary[i] - '0') * base;
        base *= 3;
    }

    return answer;
}