#include <string>
#include <vector>

using namespace std;

string solution(string s) {
    string answer = "";
    int a = s.size();
    int b = 1;
    
    if(a % 2 == 1){
        a /= 2;
    }else{
        a /= 2;
        a--;
        b++;
    }
    
    for(int i = a; i < a+b; i++){
        answer += s[i];
    }
    
    return answer;
}