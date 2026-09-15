#include <string>
#include <vector>

using namespace std;

string solution(string my_string) {
    string answer = "";
    int n = my_string.size();
    for(int i = n-1; i >= 0; i--){
        answer += my_string[i];
    }
    
    return answer;
}