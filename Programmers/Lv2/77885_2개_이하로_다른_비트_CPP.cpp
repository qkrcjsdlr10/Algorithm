#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<long long> solution(vector<long long> numbers) {
    vector<long long> answer;
    
    // cout << (numbers[1] & ~(1 << (a-1)));
    
    for(long long i = 0; i < numbers.size(); i++){
        if(numbers[i] % 2 == 0){
            answer.push_back(++numbers[i]);
        }else{
            long long a = (~numbers[i]) & (numbers[i]+1);
            long long b = numbers[i] + a - (a >> 1);
            answer.push_back(b);
        }
    }
    
    return answer;
}