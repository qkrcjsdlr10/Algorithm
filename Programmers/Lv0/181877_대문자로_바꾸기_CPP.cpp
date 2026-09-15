#include <string>
#include <vector>

using namespace std;

string solution(string myString) {
    string answer = "";
    
    for(auto c : myString){
        if(c < 'a'){
            answer += c;
        }else{
            answer += c + ('A' - 'a');
        }
    }
    
    return answer;
}