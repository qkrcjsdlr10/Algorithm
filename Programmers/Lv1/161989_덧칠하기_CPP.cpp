#include <string>
#include <vector>

using namespace std;

int solution(int n, int m, vector<int> section) {
    int answer = 0;
    
    int idx = 0;
    
    while(idx < section.size()){
        int start = section[idx];
        answer++;
        int end = section[idx] + m - 1;
        
        while(idx < section.size() && section[idx] <= end){
            idx++;
        }
        
    }
    
    return answer;
}