#include <string>
#include <vector>
#include <iostream>

using namespace std;

void reverseString(int start, int end, string& str){
    if(start >= end){
        return;
    }
    char temp = str[start];
    str[start] = str[end];
    str[end] = temp;
    reverseString(start + 1, end - 1, str);
}

vector<int> solution(string s) {
    vector<int> answer;
    
    int onesCount = 0;
    int zerosCount = 0;
    int transformCount = 0;
    int removedZeros = 0;
    string current = s;
    int decimalValue = 0;
    
    while(true){
        transformCount++;
        
        for(auto c : current){
            if(c == '1'){
                onesCount++;
            }else{
                zerosCount++;
                removedZeros++;
            }
        }

        current = "";
        while(onesCount != 0){
            current += to_string(onesCount % 2);
            onesCount /= 2;
        }
        
        reverseString(0, current.size()-1, current);
        
        for(int i = current.size()-1; i >= 0; i--){
            int power = 1;
            if(current[i] == '1'){
                int idx = i;
                if(i == current.size()-1){
                    decimalValue += 1;
                    continue;
                }
                while(idx >= 0){
                    power *= 2;
                    idx--;
                }
                decimalValue += power;
            }
        }
        
        if(current == "1"){
            break;
        }
    }
    
    answer.push_back(transformCount);
    answer.push_back(removedZeros);
    
    return answer;
}