#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solution(string dartResult) {
    int answer = 0;
    
    string str = "";
    vector<string> s;
    int idx = 0;
    int arr[5] = {0};
    vector<int> arr1;
    
    for(auto c : dartResult){
        if(c != '*' && c != '#'){
            str += c;
        }else{
            str = c;
            s.push_back(str);
            str = "";
        }
        if(str[str.size() - 1] >= 'A' && 
           str[str.size() - 1] <= 'Z' && 
           !str.empty()){
            s.push_back(str);
            str = "";
        }
    }
    
    for(auto ss : s){
        string score = "";
        if(ss == "#" && ss == "*") continue;
        for(int i = 0; i < ss.size() - 1; i++){
            score += ss[i];
            // cout << ss[i] << ' ';
        }
        
        if(ss[ss.size() - 1] == 'S' && ss.size() > 1){
            int sc = stoi(score);
            arr[idx] = sc;
            cout << arr[idx] << ' ' << idx << '\n';
            idx++;
        }else if(ss[ss.size() - 1] == 'D' && ss.size() > 1){
            int sc = stoi(score);
            arr[idx] = sc * sc;
            cout << arr[idx] << ' ' << idx << '\n';
            idx++;
        }else if(ss[ss.size() - 1] == 'T' && ss.size() > 1){
            int sc = stoi(score);
            arr[idx] = sc * sc * sc;
            cout << arr[idx] << ' ' << idx << '\n';
            idx++;
        }
        
        // cout << ss << ' ' << idx << ' ';
        
    }
    
    // if(ss == "*"){
    //         int res = 0;
    //         for(int i = idx-1; i >= idx-2; i--){
    //             if(i < 0 || s[i] == "#") break;
    //             answer += arr[i] * arr[i];
    //             cout << i << ' ';
    //         }
    //         idx++;
    //         continue;
    //     }else if(ss == "#"){
    //         // answer -= arr[idx - 1];
    //     }
    
    
    for(int i = 0; i < 3; i++){
        cout << arr[i] << ' ';
    }
    cout << '\n';
    
    idx = 0;
    for(int i = 0; i < s.size(); i++){
        if(s[i] == "*"){
            cout << i << '\n';
            for(int j = idx-1; j >= idx-2; j--){
                if(i < 0) break;
                cout << j;
                arr[j] += arr[j];
            }
            continue;
        }else if(s[i] == "#"){
            continue;
        }
        idx++;
    }
    
    cout << '\n';
    
    for(int i = 0; i < 3; i++){
        cout << i << ' ' << arr[i] << '\n';
    }
    cout << '\n';
    idx = 0;
    
    for(int i = 0; i < s.size(); i++){
        if(idx > 2 && idx < 0) break;
        if(s[i] == "*"){
            // idx++;
            continue;
        } 
        if(s[i] == "#"){
            cout << "asdf";
            cout << idx-1 << ' ' << arr[idx-1] << '\n';
            answer -= arr[idx-1];
            arr1.push_back(arr[idx-1]);
            continue;
            // idx++;
        }
        // answer += arr[idx];
        cout << idx << '\n';
        idx++;
    }
    
    for(auto df : arr){
        for(auto ff : arr1){
            if(df == ff){
                answer -= ff;
                continue;
            } 
            
        }
        answer += df;
    }
    
    
    return answer;
}