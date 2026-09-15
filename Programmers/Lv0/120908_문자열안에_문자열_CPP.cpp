#include <string>
#include <vector>

using namespace std;

int solution(string str1, string str2) {
    int answer = 0;
    
    // if(str1.find(str2) != string::npos){
    //     answer = 1;
    // }else{
    //     answer = 2;
    // }
    bool check = false;
    for(int i = 0; i < str1.size(); i++){
        string str = "";
        for(int j = i; j < i+str2.size(); j++){
            str.push_back(str1[j]);
        }
        if(str == str2){
            check = true;
            answer = 1;
            break;
        }
    }
    if(check == false){
        answer = 2;
    }
    
    return answer;
}