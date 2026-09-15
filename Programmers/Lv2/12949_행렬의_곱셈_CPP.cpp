#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<vector<int>> solution(vector<vector<int>> arr1, vector<vector<int>> arr2) {
    vector<vector<int>> answer;
    int x = 0;
    int y = 0;
    answer.resize(arr1.size(), vector<int>(arr2[0].size()));
    
    for(int i = 0; i < arr1.size(); i++){
        x = 0;
        y = 0;
        for(int j = 0; j < arr2[0].size(); j++){
            y = 0;
            int sum = 0;
            for(int k = 0; k < arr1[0].size(); k++){
                // cout << arr1[i][k] << ' ';
                // cout << arr2[y][x] << ' ';
                // cout << y << ' ' << x << ' ';
                sum += arr1[i][k] * arr2[y][x]; 
                y++;
                // cout << sum << ' ';
                // cout << '\n';
            }
            cout << '\n';
            answer[i][j] = sum;
            x++;
            // cout << sum << ' ';
            
        }
        cout << '\n';
        
    }
    
    return answer;
}