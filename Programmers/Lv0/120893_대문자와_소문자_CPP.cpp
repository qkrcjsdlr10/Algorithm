#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string my_string) {
    string answer = "";
    
    for(auto a : my_string){
        if((int)a >= 97){
            answer.push_back(a - 32);
        }else{
            answer.push_back(a + 32);
        }
    }
        
    // cout << (int)'a';
    return answer;
}