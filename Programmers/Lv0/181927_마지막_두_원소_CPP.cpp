#include <string>
#include <vector>

using namespace std;

vector<int> solution(vector<int> num_list) {
    vector<int> answer;
    
    answer = num_list;
    
    int sz = num_list.size();
    if(num_list[sz-1] > num_list[sz-2]){
        answer.push_back(num_list[sz-1] - num_list[sz-2]);
    }else{
        answer.push_back(num_list[sz-1]*2);
    }
    
    return answer;
}