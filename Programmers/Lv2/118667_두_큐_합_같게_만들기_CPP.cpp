#include <string>
#include <vector>
#include <iostream>
#include <queue>

using namespace std;

int solution(vector<int> queue1, vector<int> queue2) {
    int answer = -2;
    queue<long long> q1;
    queue<long long> q2;
    long long sum = 0;
    int cnt = 0;
    long long subSum = 0;
    long long target = 0;
    
    for(auto a : queue1){
        sum += a;
        q1.push(a);
        subSum += a;
    }
    for(auto a : queue2){
        sum += a;
        q2.push(a);
    }
    
    cout << sum << ' ';
    target  = sum / 2;
    if(sum % 2 == 1) return -1;
    int idx = 0;
    while(subSum != target && cnt < 3*queue1.size()){
        if(subSum > target){
            int temp = q1.front();
            q1.pop();
            q2.push(temp);
            subSum -= temp;
        }else{
            int temp = q2.front();
            // idx++;
            q1.push(temp);
            subSum += temp;
            q2.pop();
        }
        cnt++;
    }
    cout << subSum << ' ' << cnt;
    answer = (subSum == target) ? cnt : -1;
    // answer = cnt;
    return answer;
}