#include <string>
#include <vector>

using namespace std;

string solution(int n) {
    string answer = "";
    
    while(n > 0){
        int remainder = n % 3;
        n /= 3;
        if(remainder == 0){
            n--;
            remainder = 4;
        }
        answer = to_string(remainder) + answer;
    }
    
    return answer;
}