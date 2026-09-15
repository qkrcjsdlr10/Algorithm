#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

int index = 0;

bool cmp(vector<int> &arr1, vector<int> &arr2){
    if(arr1[index] != arr2[index]) return arr1[index] < arr2[index];
    if(arr1[index] == arr2[index]) return arr1[0] > arr2[0];
}

int solution(vector<vector<int>> data, int col, int row_begin, int row_end) {
    int answer = 0;
    
    index = col-1;
    
    sort(data.begin(), data.end(), cmp);
    
    // cout << index;
    // for(auto ar : data){
    //     for(auto a : ar){
    //         cout << a << ' ';
    //     }
    //     cout << '\n';
    // }
    
    vector<int> res;
    
    for(int i = row_begin-1; i <= row_end-1; i++){
        int a = 0;
        for(int j = 0; j < data[i].size(); j++){
            a += (data[i][j] % (i+1));
            // cout << data[i][j];
        }
        // cout << a << ' ';
        res.push_back(a);
    }
    
    for(auto a : res){
        answer ^= a;
    }
    
    
    
    return answer;
}