#include <string>
#include <vector>
using namespace std;

vector<int> solution(int n, long long k) {
    vector<int> answer;
    vector<int> num;
    
    long long fact = 1;
    
    for(int i=1;i<=n;i++){
        num.push_back(i);
        fact *= i;
    }
    
    k--; // 0-index
    
    for(int i=n;i>=1;i--){
        fact /= i;
        
        int idx = k / fact;
        answer.push_back(num[idx]);
        
        num.erase(num.begin() + idx);
        
        k %= fact;
    }
    
    return answer;
}