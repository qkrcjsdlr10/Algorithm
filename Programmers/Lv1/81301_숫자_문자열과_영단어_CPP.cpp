#include <string>
#include <vector>
#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int func(vector<string> str, string sum){
    for(int i = 0; i < str.size(); i++){
        if(str[i] == sum){
            return i;
        }
    }
    return -1;
}

int solution(string s) {
    int answer = 0;
    int cnt = 0;
    string res = "0";
    vector<string> str;
    str.push_back("zero");
    str.push_back("one");
    str.push_back("two");
    str.push_back("three");
    str.push_back("four");
    str.push_back("five");
    str.push_back("six");
    str.push_back("seven");
    str.push_back("eight");
    str.push_back("nine");
    string sum = "";
    for(int i = 0; i < s.size(); i++){
        if(s[i] >= 'a' && s[i] <= 'z'){
            sum += s[i];
            int c = func(str, sum);
            if(c >= 0){
                res += to_string(c);
                cout << c << ' ';
                sum = "";
                continue;
            }
        }else{
            // cout << s[i] << ' ';
            res += s[i];
            
        }
    }
    
    answer = stoi(res);
    
    return answer;
}