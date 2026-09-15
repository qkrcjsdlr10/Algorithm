#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int a, int b, int n) {
    int answer = 0;
    int remain = 0;
    
    while(n >= a){
        int remain = n / a;
        answer += remain * b;
        n = (n % a) + (remain * b);
    }
    
    // n += remain;
    // remain = 0;
    // while(1){
    //     if(n >= a){
    //         answer += b;
    //         n -= a;
    //         n+=b;
    //     }else{
    //         break;
    //     }
    // }
    
    
    
    
    return answer;
}