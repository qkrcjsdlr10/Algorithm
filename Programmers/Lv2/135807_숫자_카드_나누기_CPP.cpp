#include <string>
#include <vector>

using namespace std;

int gcd(int a, int b){
    while(b != 0){
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

int solution(vector<int> arrayA, vector<int> arrayB) {
    int answer = 0;
    
    int a = arrayA.size();
    int b = arrayB.size();
    
    int ag = arrayA[0];
    for(int i = 1; i < a; i++){
        ag = gcd(ag, arrayA[i]);
    }
    
    bool acheck = false;
    
    for(int i = 0; i < b; i++){
        if(arrayB[i] % ag == 0){
            acheck = true;
            break;
        } 
    }
    
    int bg = arrayB[0];
    bool bcheck = false;
    for(int i = 1; i < b; i++){
        bg = gcd(bg, arrayB[i]);
    }
    for(int i = 0; i < a; i++){
        if(arrayA[i] % bg == 0){
            bcheck = true;
            break;
        }
    }
    
    answer = (!bcheck || !acheck)? max(bg, ag) : 0;
    
    return answer;
}