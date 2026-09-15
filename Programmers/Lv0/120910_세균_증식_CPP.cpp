#include <string>
#include <vector>

using namespace std;

int solution(int n, int t) {
    int answer = 0;
    int sum = n;
    for(int i = 0; i < t; i++){
        sum = sum * 2;
    }
    answer = sum;
    return answer;
}