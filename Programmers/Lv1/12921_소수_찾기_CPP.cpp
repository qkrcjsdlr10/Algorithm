#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int n) {
    int answer = 0;
    
    int arr[1000000] = {0};
    
    for(int i = 2; i <= n; i++){
        int a = i;
        while(a <= n){
            // cout << a << ' ';
            arr[a]++;
            a += i;
            // cout << a << ' ';
        }
        // cout << '\n';
    }
    
    for(int i = 2; i <= n; i++){
        if(arr[i] == 1){
            // cout << i << ' ';
            answer++;
        }
    }
    
    return answer;
}