#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int a = 0;

bool cmp(vector<int> &arr1, vector<int> &arr2){
    return arr1[a] < arr2[a];
}


vector<vector<int>> solution(vector<vector<int>> data, string ext, int val_ext, string sort_by) {
    vector<vector<int>> answer;
    
    vector<string> s;
    s.push_back("code");
    s.push_back("date");
    s.push_back("maximum");
    s.push_back("remain");
    
    auto it = find(s.begin(), s.end(), ext);
    
    int idx1 = it - s.begin();
    vector<vector<int>> arr;
    
    for(int i = 0; i < data.size(); i++){
        if(data[i][idx1] < val_ext){
            arr.push_back(data[i]);
        }
    }
    
    // for(auto ar : arr){
    //     for(auto a : ar){
    //         cout << a << ' ';
    //     }
    //     cout << '\n';
    // }
    auto it1 = find(s.begin(), s.end(), sort_by);
    
    a = it1 - s.begin();
    cout  << a;
    sort(arr.begin(), arr.end(), cmp);
    
    for(auto a : arr){
        answer.push_back(a);
    }
    
    return answer;
}