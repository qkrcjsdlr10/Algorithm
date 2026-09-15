#include <string>
#include <vector>

using namespace std;

int solution(vector<int> num_list) {
    int answer = 0;
    
    int cnt = 0;
    
    for(auto n : num_list){
        if(n < 0){
            break;
        }else{
            cnt++;
        }
    }
    answer = (cnt == num_list.size()) ? -1 : cnt;
    return answer;
}