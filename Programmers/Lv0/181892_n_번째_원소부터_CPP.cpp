#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> num_list, int n) {
    vector<int> answer;
    
    int cnt = 1;
    
    for(auto a : num_list){
        if(cnt >= n){
            answer.push_back(a);
        }else{
            cnt++;
        }
    }
    
    return answer;
}