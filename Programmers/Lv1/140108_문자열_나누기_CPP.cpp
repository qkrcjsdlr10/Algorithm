#include <string>
#include <vector>
#include <iostream>
using namespace std;

int solution(string s) {
    int answer = 0;
    string sub = "";
    int cur = 0;
    int cnt = 0;
    
    while(cur < s.size() && cur + cnt < s.size()){
        int t = 0, f = 0;
        char ch = ' ';
        cnt = 0;
        ch = s[cur];
        for(int i = cur; i < s.size(); i++){
            if(t != 0 && t == f){
                answer++;
                cout << sub << ' ' << cnt << '\n';
                sub = "";
                cur += cnt;
                cnt = 0;
                break;
            }
            // cout << s[i];
            if(ch == s[i]){
                t++;
            }else{
                f++;
            }
            sub += s[i];
            cnt++;
        }
    }
    cout << cur;
    
    return ++answer;
}