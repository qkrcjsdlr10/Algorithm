#include <string>
#include <vector>
#include <iostream>
using namespace std;

int solution(int n) {
    int answer = 0;
    int a = n;
    int cnt = 0;
    while(a > 0){
        if(a % 2 == 1){
            cnt++;
        }
        a /= 2;
    }
    cout << cnt;
    a = n;
    
    while(true){
        a++;
        int div = a;
        int rcnt = 0;
        while(div > 0){
            if(div % 2 == 1){
                rcnt++;
            }
            div /= 2;
        }
        if(rcnt == cnt){
            break;
        }
    }
    
    answer = a;
    return answer;
}