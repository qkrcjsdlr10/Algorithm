#include <vector>
#include <iostream>
using namespace std;

int solution(vector<int> nums) {
    int answer = 0;
    int n = 2;
    vector<bool> check(50001, true);
    check[0] = check[1] = false;
    
    for(int i = 2; i * i <= 50001; i++){
        if(check[i]){
            for(int j = i*i; j <= 50001; j += i){
                check[j] = false;
            }
        }
    }
    cout << check[7];
    
    for(int i = 0; i < nums.size() - 2; i++){
        for(int j = i + 1; j < nums.size() - 1; j++){
            for(int k = j + 1; k < nums.size(); k++){
                int sum = nums[i] + nums[j] + nums[k];
                // cout << sum;
                if(check[sum] == true){
                    cout << check[sum] << ' ';
                    answer++;
                }
            }
        }
    }

    return answer;
}