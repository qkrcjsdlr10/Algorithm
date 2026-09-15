#include <string>
#include <vector>

using namespace std;

int solution(int n, int k) {
    int answer = 0;
    
    int sum = 12000 * n;
    k = k - n/10;
    sum += k * 2000;
    answer = sum;
    
    return answer;
}