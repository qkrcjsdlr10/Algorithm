#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int solution(vector<int> citations) {
    int answer = 0;
    //0 1 3 5 6       4 3 2 1 0
    sort(citations.rbegin(), citations.rend());
    
    for(int i = 0; i < citations.size(); i++){
        if(citations[i] >= i+1){
            answer = i+1;
        }
    }
    
    return answer;
}