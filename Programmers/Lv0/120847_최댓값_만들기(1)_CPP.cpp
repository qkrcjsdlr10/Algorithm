#include <string>
#include <vector>

using namespace std;

int solution(vector<int> numbers) {
    int answer = 0;
    int n = numbers.size();
    
    for(int i = 0; i < n-1; i ++){
        for(int j = 0; j < n-1-i; j++){
            if(numbers[j] > numbers[j+1]){
                int temp = numbers[j];
                numbers[j] = numbers[j+1];
                numbers[j+1] = temp;
            }
        }
    }
    return numbers[n-1] * numbers[n-2];
}