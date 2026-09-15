#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

string solution(string new_id) {
    string answer = "";
    
    for(int i = 0; i < new_id.size(); i++){
        if(new_id[i] >= 'A' && new_id[i] <= 'Z'){
            new_id[i] = tolower(new_id[i]);
        }
    }
    // cout << new_id;
    int idx = 0;
    while(idx < new_id.size()){
        if(new_id[idx] >= 'a' && new_id[idx] <= 'z'){
            idx++;
            continue;
        }
        if(new_id[idx] >= '0' && new_id[idx] <= '9'){
            idx++;
            continue;
        }
        if(new_id[idx] == '.' || new_id[idx] == '-' || new_id[idx] == '_'){
            idx++;
            continue;
        }
        new_id.erase(idx, 1);
    }
    
    cout << new_id << '\n';
    
    idx = 0;
    int cnt = 0;
    int start = 0;
    
    // while(idx+1 < new_id.size()){
    //     if(new_id[idx] == '.' && new_id[idx+1] == '.'){
    //         if(cnt == 0){
    //             start = idx;
    //         }
    //         cnt++;
    //     }else{
    //         if(cnt != 0){
    //             new_id.erase(start, cnt+1);
    //             cnt = 0;
    //             idx = start;
    //             continue;
    //             // cout << new_id << '\n';
    //         }
    //     }
    //     idx++;
    // }
    
    while(idx + 1 < new_id.size()){
        if(new_id[idx] == '.' && new_id[idx+1] == '.'){
            if(cnt == 0){
                start = idx;
            }
            cnt++;
        }else{
            if(cnt != 0){
                new_id.erase(start, cnt);
                cnt = 0;
                idx = max(0, start - 1);
                continue;
            }
        }
        idx++;
    }

    if(cnt != 0){
        new_id.erase(start, cnt+1);
    }
    
    // idx = 0;
    // while (idx + 1 < new_id.size()) {
    //     if (new_id[idx] == '.' && new_id[idx + 1] == '.') {
    //         new_id.erase(idx, 1);
    //     } else {
    //         idx++;
    //     }
    // }
    
//     string temp;

//     for(char c : new_id){
//         if(c == '.' && !temp.empty() && temp.back() == '.') continue;
//         temp += c;
//     }

//     new_id = temp;
    
    
    if(new_id[0] == '.'){
        new_id.erase(0, 1);
    }
    if(new_id[new_id.size()-1] == '.'){
        new_id.erase(new_id.size()-1, 1);
    }
    
    if(new_id.size() == 0){
        new_id = 'a';
    }
    if(new_id.size() > 15){
        new_id = new_id.substr(0, 15);
    }
    
    if(new_id[new_id.size()-1] == '.'){
        new_id.erase(new_id.size()-1, 1);
    }
    
    while(new_id.size() < 3){
        new_id += new_id[new_id.size()-1];
    }
    
    answer = new_id;
    cout << new_id;
    return answer;
}