#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(string str1, string str2) {
    int answer = 0;
    
    vector<string> sub1;
    vector<string> sub2;
    vector<string> inter;
    vector<string> uni;
    
    for(int i = 0; i < str1.size()-1; i++){
        string substr = "";
        for(int j = i; j < i+2; j++){
            char c = str1[j];
            if(!((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A')))
                break;
            c = tolower(c);
            substr += c;
        }
        if(substr.size() == 2)
            sub1.push_back(substr);
    }
    
    for(int i = 0; i < str2.size()-1; i++){
        string substr = "";
        for(int j = i; j < i+2; j++){
            char c = str2[j];
            if(!((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A')))
                break;
            c = tolower(c);
            substr += c;
        }
        if(substr.size() == 2)
            sub2.push_back(substr);
    }
    
    for(int i = 0; i < sub1.size(); i++){
        bool check = false;
        for(int j = 0; j < sub2.size(); j++){
            if(sub1[i] != "" && sub2[j] != ""){
                if(sub1[i] == sub2[j]){
                    inter.push_back(sub1[i]);
                    uni.push_back(sub1[i]);
                    sub1[i] = "";
                    sub2[j] = "";
                    check = true;
                }
            }
        }
        if(check == false){
            uni.push_back(sub1[i]);
        }
    }
    for(auto s : sub2){
        if(s != ""){
            uni.push_back(s);
        }
    }
    
    // for(auto s : inter){
    //     cout << s << ' ';
    // }
    // cout << '\n';
    // for(auto s : uni){
    //     cout << s << ' ';
    // }
    double res;
//     cout << '\n';
    
    if(uni.size() != 0){
        res = (inter.size()*1.0/uni.size());
    }else{
        res = 1.0;
    }
    
    // cout << inter.size() << ' ' << uni.size() << ' ' << res * 65536;
    // cout << '\n';
    res *= 65536;
    // cout << res;
    answer = (int)res;
    return answer;
}