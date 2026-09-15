#include <string>
#include <vector>

using namespace std;

int solution(vector<vector<string>> clothes) {
    int answer = 0;
    vector<string> arr;
    vector<int> arr2;
    
    for(int i = 0; i < clothes.size(); i++){
        bool check = false;
        int idx = -1;
        for(int j = 0; j < arr.size(); j++){
            if(arr[j] == clothes[i][1]){
                check = true;
                idx = j;
                break;
            }
        }    
        if(!check){
            arr.push_back(clothes[i][1]);
            arr2.push_back(1);
        }else{
            if(idx != -1){
                arr2[idx]++;    
            }
            
        }
    }
    
    answer = 1;
    for(int i = 0; i < arr2.size(); i++){
        answer *= (arr2[i] + 1);
    }
    answer--;
    
    return answer;
}