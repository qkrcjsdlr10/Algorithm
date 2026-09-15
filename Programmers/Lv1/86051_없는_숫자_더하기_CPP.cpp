#include <string>
#include <vector>

using namespace std;

int solution(vector<int> numbers) {
    int answer = -1;
    int arr[11] = {0};
    int sum = 0;
    
    for(int i = 0; i < numbers.size(); i++){
        arr[numbers[i]]++;
    }
    
    for(int i = 0; i < 10; i++){
        if(arr[i] == 0){
            sum += i;
        }
    }
    answer = sum;
    return answer;
}