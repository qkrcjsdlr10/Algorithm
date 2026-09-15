#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int number, int limit, int power) {
    int answer = 0;
    int cnt = 1;
    int div = 1;
    vector<int> divisorCount(number + 1, 1); // 1은 모든 수의 약수
    for (int i = 2; i <= number; ++i) {
        for (int j = i; j <= number; j += i) {
            divisorCount[j]++;
        }
    }
    
    for(int i = 1; i <= number; i++){
        int div = 1;
        int cnt = 1;
        // while(i > div){
        //     if(i % div == 0){
        //         cnt++;
        //         div++;
        //     }else{
        //         div++;
        //     }
        // }
        // cout << cnt << ' ';
        
        
        if(divisorCount[i] > limit){
            answer += power;
        }else{
            answer += divisorCount[i];
        }
    }
    
    // while(5 >= div){
    //     if(5 % div == 0){
    //         cnt++;
    //         cout << div << ' ';
    //         div++;
    //     }else{
    //         div++;
    //     }
    // }
    
    return answer;
}