#include <string>
#include <vector>

using namespace std;

vector<int> solution(int n, int m) {
    vector<int> answer;
    
    for(int i = m; i > 0; i--){
        if(n % i == 0 && m % i == 0){
            answer.push_back(i);
            break;
        }
    }
    
    int i = 1;
    
    while(1){
        if(i % n == 0 && i % m == 0){
            answer.push_back(i);
            break;
        }
        i++;
    }
    
    return answer;
}