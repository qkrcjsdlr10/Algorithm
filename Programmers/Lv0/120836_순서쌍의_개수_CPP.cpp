#include <string>
#include <vector>
#include <iostream>
using namespace std;

int solution(int n) {
    int answer = 0;
    
    int cnt = 1;
    
    while(cnt <= n/2){
        if(n % cnt == 0) answer++;
        // cout <<  cnt << ' ';
        cnt++;
    }
    answer++;
    return answer;
}