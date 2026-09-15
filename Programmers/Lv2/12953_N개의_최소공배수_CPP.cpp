#include <string>
#include <vector>

using namespace std;

int lcm(int a, int b){
    while(b != 0){
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int gcb(int a, int b){
    return a / (lcm(a, b)) * b;
}

int solution(vector<int> arr) {
    int answer = 0;
    answer = arr[0];
    
    for(int i = 0; i < arr.size(); i++){
        answer = gcb(answer, arr[i]);    
    }
    
    return answer;
}