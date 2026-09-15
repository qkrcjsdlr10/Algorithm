#include <string>
#include <iostream>
using namespace std;

bool solution(string s)
{
    bool answer = false;
    int a = 0;
    int b = 0;
    
    for(int i = 0; i < s.size(); i++){
        char c = tolower(s[i]);
        if(c == 'y'){
            a++;
        }
        if(c == 'p'){
            b++;
        }
    }
    
    if(a == b){
        answer = true;
    } 
    
    // [실행] 버튼을 누르면 출력 값을 볼 수 있습니다.
    // cout << "Hello Cpp" << endl;

    return answer;
}