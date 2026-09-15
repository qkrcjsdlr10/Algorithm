#include <string>
#include <vector>
#include <queue>

using namespace std;

int solution(int n, int k, vector<int> enemy) {
    int answer = 0;
    
    priority_queue<int> pq;
    
    for(int i = 0; i < enemy.size(); i++){
        n -= enemy[i];
        pq.push(enemy[i]);
        if(n < 0){
            if(k > 0){
                int temp = pq.top(); pq.pop();
                n += temp;
                k--;
            }else{
                break;
            }
            
        }
        answer++;
    }
    
    
    return answer;
}