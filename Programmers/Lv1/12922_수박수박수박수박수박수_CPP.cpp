#include <string>
#include <vector>

using namespace std;

string solution(int n) {
    string answer = "";
    
    if(n > 0){
        answer += "수";
    }
    int cnt = 0;
    while(n > cnt+1){
        if(cnt % 2 == 0){
            answer += "박";
        }else{
            answer += "수";
        }
        cnt++;
    }
    
    return answer;
}