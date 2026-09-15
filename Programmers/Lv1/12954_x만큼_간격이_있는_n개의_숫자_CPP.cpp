#include <string>
#include <vector>

using namespace std;

vector<long long> solution(int x, int n) {
    vector<long long> answer;
    int a = 0;
    while(n >0){
        a += x;
        answer.push_back(a);
        n--;
    }
    
    return answer;
}