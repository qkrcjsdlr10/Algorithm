#include <string>
#include <vector>

using namespace std;

bool check(string s){
    int count = 0;
    for(auto c : s){
        if(c == '('){
            count++;
        }else{
            count--;
        }
        if(count < 0) return false;
    }
    return (count == 0) ? true : false;
}

string dfs(string s){
    if (s.empty()) return "";
    
    string u = "";
    string v = "";
    int idx = 0;
    int left = 0;
    int right = 0;
    
    for(int i = 0; i < s.size(); i++){
        if(s[i] == '('){
            left++;
        }else{
            right++;
        }
        if(left == right){
            idx = i;
            break;
        } 
    }
    
    u = s.substr(0, idx+1);
    v = s.substr(idx+1);
    
    if(check(u)){
        return u + dfs(v);
    }else{
        string res = "";
        res = '(' + dfs(v) + ')';
        for(int i = 1; i < u.size()-1; i++){
            if(u[i] == '('){
                res += ')';
            }else{
                res += '(';
            }
        }
        return res;
    }
}

string solution(string p) {
    string answer = "";
    
    answer = dfs(p);
    
    return answer;
}