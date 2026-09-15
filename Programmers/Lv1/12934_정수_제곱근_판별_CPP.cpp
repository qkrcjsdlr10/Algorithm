#include <string>
#include <vector>

using namespace std;

long long solution(long long n) {
    long long answer = 0;
    
    long long a = 1;
    long long b = 0;
    
    while(1){
        b = a*a;
        if(b >= n){
            break;
        }
        a++;
    }
    a++;
    if(b == n){
        answer = a*a;
    }else{
        answer = -1;
    }
    
    return answer;
}