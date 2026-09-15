#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

int n = 0;
int res = 0;

void dfs(vector<int> numbers, int depth, int target, int k){
    if(depth == n){
        if(k == target){
            res++;
        }
        return;
    }
    // cout << k << ' ';
    dfs(numbers, depth + 1, target, k + numbers[depth]);
    dfs(numbers, depth + 1, target, k - numbers[depth]);
}

int solution(vector<int> numbers, int target) {
    int answer = 0;
    
    n = numbers.size();
    
    dfs(numbers, 0, target, 0);
    
    cout << res;
    answer = res;
    return answer;
}