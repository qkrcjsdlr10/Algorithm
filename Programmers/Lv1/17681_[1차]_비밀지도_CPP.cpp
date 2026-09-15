#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> solution(int n, vector<int> arr1, vector<int> arr2) {
    vector<string> answer;
    
    vector<vector<char>> arr;
    arr.resize(n, vector<char> (n));
    
    for(int i = 0; i < arr1.size(); i++){
        int a = arr1[i];
        int b = arr2[i];
        string s = "";
        for(int j = 0; j < arr1.size(); j++){
            if(a % 2 == 0 && b % 2 == 0){
                s = "0" + s;
            }else{
                s = "1" + s;
            }
            a /= 2;
            b /= 2;
            // cout << s << ' ';
        }
        cout << s << '\n';
        string str = "";
        for(int j = 0; j < arr1.size(); j++){
            if(s[j] == '0'){
                arr[i][j] = ' ';
                str += ' ';
            }else{
                arr[i][j] = '#';
                str += '#';
            }
        }
        // cout << str << '\n';
        answer.push_back(str);
    }
    
    return answer;
}