#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int a, int b) {
    int answer = 0;
    
    int c = b;
    int d = a;
    int cnt = 0;
    
    while(c > 0){
        c /= 10;
        d*=10;
        cnt++;
    }
    if(d + b > 2 * a * b){
        answer = d + b;
    }else{
        answer = 2 * a * b;
    }
    cout << d;
    return answer;
}