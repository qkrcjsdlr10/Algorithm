#include <string>
#include <vector>

using namespace std;

//1 3 6 10 15 20 26 32 39

int solution(int n) {
    int answer = 0;
    
    for(int i = 1; i <= n; i++){
        int sum = 0;
        for(int j = i; j <= n; j++){
            sum += j;
            if(sum == n){
                answer++;
                break;
            }
            if(sum > n){
                break;
            }
        }
    }
    
    return answer;
}