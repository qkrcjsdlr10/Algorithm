#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> solution(vector<string> strings, int n) {
    vector<string> answer;
    
    bool check = false;
    
    check = false;
    for(int i = 0; i < strings.size() - 1; i++){
        for(int j = 0; j < strings.size() - 1 - i; j++){
            if(strings[j][n] > strings[j+1][n]){
                string temp = strings[j];
                strings[j] = strings[j+1];
                strings[j+1] = temp;
            }else if(strings[j][n] == strings[j+1][n]){
                if(strings[j] > strings[j+1]){
                    string temp = strings[j];
                    strings[j] = strings[j+1];
                    strings[j+1] = temp;
                }
            }
        }
    }
    

    
    return strings;
}