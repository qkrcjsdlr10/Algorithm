#include <vector>
#include <algorithm>
using namespace std;

long long solution(long long n) {
    vector<int> digits;
    int temp = 0;
    // 자릿수 분리
    while(n > 0) {
        digits.push_back(n % 10);
        n /= 10;
    }
    
    // 내림차순 정렬
    for(int i = 0; i < digits.size(); i++){
        for(int j = i+1; j < digits.size(); j++){
            if(digits[i] < digits[j]){
                temp = digits[i];
                digits[i] = digits[j];
                digits[j] = temp;
            }
        }
    }
    
    // 숫자 재조합
    long long answer = 0;
    for(int d : digits) {
        answer = answer * 10 + d;
    }
    
    return answer;
}
