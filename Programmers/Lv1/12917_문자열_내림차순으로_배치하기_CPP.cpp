#include <string>
#include <vector>

using namespace std;

string solution(string s) {
    string answer = "";
    
    for(int i = 0; i < s.size()-1; i++){
        for(int j = i+1; j < s.size(); j++){
            if(s[i] - 'a' < s[j] - 'a'){
                char temp = s[i];
                s[i] = s[j];
                s[j] = temp;
            }
        }
    }
    
    for(int i = 0; i < s.size(); i++){
        answer += s[i];
    }
    
    return answer;
}