#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int n) {
    int answer = 0;
    
    int a = 0;
    int b = 0;
    
    for(int i = 1; i <= n; i++){
        if(i % 2 == 0){
            a += i*i;
        }else{
            b += i;
        }
    }
    cout << a << ' ' << b;
    answer = (n % 2 == 0) ? a : b;
    
    return answer;
}