#include <string>
#include <vector>

using namespace std;

int solution(int n) {
    int answer = 0;
    
    while(n > 0){
        int div = n % 10;
        n /= 10;
        answer += div;
    }
    
    return answer;
}