#include <string>
#include <vector>

using namespace std;

bool solution(int x) {
    bool answer = true;
    int a = 0;
    int b = x;
    int idx = 0;
    int sum = 0;
    int arr[10] = {0};
    
    while(b > 0){
        a = b%10;
        b /= 10;
        arr[idx++] = a;
    }
    
    for(int i = 0; i < idx; i++){
        sum += arr[i];
    }
    
    if(x % sum == 0){
        answer = true;
    }else{
        answer = false;
    }
    
    
    return answer;
}