#include <string>
#include <vector>

using namespace std;

int solution(vector<int> num_list) {
    int answer = 0;
    
    int a = 0;
    int b = 1;
    
    for(auto n : num_list){
        a += n;
        b *= n;
    }
    answer = (a*a > b) ? 1 : 0;
    return answer;
}