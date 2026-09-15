#include <string>
#include <vector>

using namespace std;

string solution(string s) {
    string answer = "";
    
    for(auto c : s){
        if(c == ' '){
            answer += ' ';
            continue;
        }
        
        if(answer.size() == 0){
            if(c >= 'a' && c <= 'z'){
                answer += c - 32;
            }else{
                answer += c;
            }
            continue;
        }
        
        if(c >= '0' && c <= '9'){
            answer += c;
            continue;
        }
        
        if(answer[answer.size()-1] == ' '){
            if(c >= 'a' && c <= 'z'){
                answer += c - 32;
            }else{
                answer += c;
            }
            continue;
        }
        
        if(c >= 'A' && c <= 'Z'){
            answer += c + 32;
        }else{
            answer += c;
        }
        
        
    }
    
    return answer;
}