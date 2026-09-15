#include <string>
#include <vector>

using namespace std;

int solution(int a, int b) {
    int answer = 0;
    
    string s1 = to_string(a) + to_string(b);
    string s2 = to_string(b) + to_string(a);
    
    if(stoi(s1) > stoi(s2)){
        answer = stoi(s1);
    }else{
        answer = stoi(s2);
    }
    
    return answer;
}