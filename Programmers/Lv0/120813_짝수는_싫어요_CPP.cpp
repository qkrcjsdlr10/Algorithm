#include <string>
#include <vector>

using namespace std;

vector<int> solution(int n) {
    vector<int> answer;
    int cnt = 1;
    while(cnt <= n){
        if(cnt % 2 == 1){
            answer.push_back(cnt);
        }
        cnt++;
    }
    
    return answer;
}