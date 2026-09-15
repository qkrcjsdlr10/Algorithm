#include <string>
#include <vector>

using namespace std;

int solution(int n) {
    int answer = 0;
    int sum = 0;
    
    while(n > 0){
        if(n % 2 == 0){
            sum += n;
        }
        n--;
    }
    answer = sum;
    
    return answer;
}