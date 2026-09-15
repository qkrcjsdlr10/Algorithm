#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> arr) {
    vector<int> answer;
    
    if(arr.size() <= 1){
        answer.push_back(-1);
        return answer;
    }
    
    int minNum = arr[0];
    int idx = 0;
    for(int i = 0; i < arr.size(); i++){
        if(minNum > arr[i]){
            minNum = arr[i];
            idx = i;
        }
    }
    
    for(int i = idx; i < arr.size() - 1; i++){
        arr[i] = arr[i + 1];
    }
    arr.resize(arr.size()-1);
    
    return arr;
}