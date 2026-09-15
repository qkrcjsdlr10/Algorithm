#include <vector>
using namespace std;

int solution(vector<int> nums)
{
    int answer = 0;
    int a = 0;
    int size = nums.size() / 2;
    
    int arr[200000] = {0};
    
    for(int i = 0; i < nums.size(); i++){
        arr[nums[i]]++;
    }
    
    for(int i = 0; i < 200000; i++){
        if(arr[i] != 0){
            answer++;
            a++;
        }
        if(a == size){
            break;
        }
    }
    
    return answer;
}