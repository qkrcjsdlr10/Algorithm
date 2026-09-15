#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(string my_string) {
    int answer = 0;
    
    for(auto c : my_string){
        if(c >= '0' && c <= '9'){
            answer += (int)c - 48;
        }
    }
    
    return answer;
}