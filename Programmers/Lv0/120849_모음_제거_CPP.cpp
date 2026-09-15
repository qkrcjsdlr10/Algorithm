#include <string>
#include <vector>

using namespace std;

bool find(char c){
    if(c == 'a' || c == 'e' || c == 'o' || c == 'i' || c == 'u'){
        return false;
    }else{
        return true;
    }
}

string solution(string my_string) {
    string answer = "";
    
    for(auto c : my_string){
        if(find(c)){
            answer.push_back(c);
        }
    }
    
    return answer;
}