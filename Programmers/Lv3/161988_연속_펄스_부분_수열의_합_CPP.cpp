#include <string>
#include <vector>
#include <algorithm>

using namespace std;

long long solution(vector<int> sequence) {
    long long answer = 0;
    
    vector<long long> arr1(sequence.begin(), sequence.end());
    vector<long long> arr2(sequence.begin(), sequence.end());
    
    int a = 1;
    
    for(int i = 0; i < sequence.size(); i++){
        arr1[i] *= a;
        arr2[i] *= -a;
        a *= -1;
    }
    long long cur1 = arr1[0];
    long long cur2 = arr2[0];
    answer = max(cur1, cur2);   
    
    for(int i = 1; i < sequence.size(); i++){
        cur1 = max(cur1 + arr1[i], arr1[i]);
        cur2 = max(cur2 + arr2[i], arr2[i]);
        
        answer = max(cur1, answer);
        answer = max(cur2, answer);
    }
    
    return answer;
}