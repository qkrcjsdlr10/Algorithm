#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(vector<int> array, vector<vector<int>> commands) {
    vector<int> answer;
    vector<int> arr;
    for(int i = 0; i < commands.size(); i++){
        for(int j = commands[i][0]-1; j <= commands[i][1]-1; j++){
            arr.push_back(array[j]);
            // cout << array[j];
        }
        sort(arr.begin(), arr.end());
        answer.push_back(arr[commands[i][2] - 1]);
        
        arr.clear();
    }
    
    // for(int i = 0; i < arr.size(); i++){
    //     cout << arr[i] << ' ';
    // }
    
    return answer;
}