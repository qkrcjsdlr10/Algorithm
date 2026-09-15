#include <string>
#include <vector>

using namespace std;

int solution(int num) {
    long long n = num;
    
    if (n == 1) return 0;
    
    int answer = 0;
    
    while(answer < 500){
        if(n == 1){
            break;
        }
        
        answer++;
        
        if(n % 2 == 0){
            n /= 2;
        }else{
            n = n * 3 + 1;
        }
    }
    
    if(n != 1) answer = -1;
    
    return answer;
}