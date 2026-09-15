#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int* solution(long long n) {
    long long idx = 0;
    int l = 0;
    long long a = n;
    while(a > 0){
        a/=10;
        l++;
    }
    // 리턴할 값은 메모리를 동적 할당해주세요.
    int* answer = (int*)malloc(sizeof(int) * l);
    while(n > 0){
        int num = n % 10;
        n /= 10;
        answer[idx++] = num;
    }
    
    return answer;
}