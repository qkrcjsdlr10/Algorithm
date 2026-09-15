#include <string>
#include <vector>
#include <iostream>
using namespace std;

int solution(int n) {
    int answer = 0;
    
    int a = 1;
    
    while(a*a <= n){
        if(a*a == n) answer = 1;
        // cout << a*a << ' ';
        a++;
    }
    if(answer == 0) answer = 2;
    return answer;
}