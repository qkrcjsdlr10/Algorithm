#include <bits/stdc++.h>
using namespace std;

long long solution(int price, int money, int count)
{
    long long answer = -1;
    long long sum = 0;
    
    while(count > 0){
        sum += count * price;
        count--;
    }
    
    answer = money - sum;
    if(answer >= 0){
        answer = 0;
    }else{
        answer = -answer;
    }

    return answer;
}