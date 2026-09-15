#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(string s, int n) {
    string answer = "";
    for(auto c : s){
        if(c == ' '){
            answer += ' ';
            continue;
        } 
        if(c <= 'z' && c >= 'a'){
            if(c - 'a' + n <= 'z' - 'a'){
                c += n;
                answer += c;
            }else{
                int ac = (c - 'a' + n) % 26;
                char b = 'a' + ac;
                answer += b;
            }
        }else{
            if(c - 'A' + n <= 'Z' - 'A'){
                c += n;
                answer += c;
            }else{
                int ac = (c - 'A' + n) % 26;
                char b = 'A' + ac;
                answer += b;
            }
        }
    }
    
    return answer;
}