#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> array) {
    vector<int> answer;
    
    int idx = 0;
    int mx = 0;
    for(int i = 0; i < array.size(); i++){
        mx = max(array[i], mx);
    }
    for(int i = 0; i < array.size(); i++){
        if(array[i] == mx){
            idx = i;
        }
    }
    
    return {mx, idx};
}