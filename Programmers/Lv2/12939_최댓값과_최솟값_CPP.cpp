#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

string solution(string s) {
    string answer = "";
    
    vector<int> arr;
    
    string str = "";
    for(int i  = 0; i < s.size(); i++){
        if(s[i] != ' '){
            str += s[i];
            if(i != s.size()-1){
                continue;    
            }
        }
        
        // cout << str << ' ';
        
        int a = stoi(str);
        
        arr.push_back(a);
        
        str = "";
        
    }
    
    sort(arr.begin(), arr.end());
    
    cout << arr[0] << ' ' << arr[arr.size()-1];
    // answer += arr[0] + " " + arr[arr.size()-1];
    
    answer += to_string(arr[0]) + ' ' + to_string(arr[arr.size()-1]);
    
    return answer;
}