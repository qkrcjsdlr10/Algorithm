#include <iostream>

using namespace std;

int solution(int n, int a, int b)
{
    int answer = 0;
    int cnt = 0;
    
    
    while(true){
        if((a+1)/2 == (b+1)/2){
            cnt++;
            break;
        }
        a = (a+1) / 2;
        b = (b+1) / 2;
        cnt++;
    }
    answer = cnt;
    return answer;
}