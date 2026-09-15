#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>

using namespace std;

vector<string> solution(vector<string> record) {
    vector<string> answer;
    vector<pair<string, int>> vis;
    vector<vector<string>> v;
    map<string, string> nick;
    
    for(int i = 0; i < record.size(); i++){
        string action = "";
        string name = "";
        string uid = "";
        int cnt = 0;
        vector<string> sub;
        
        for(int j = 0; j < record[i].size(); j++){
            if(cnt == 0 && record[i][j] != ' '){
                action += record[i][j];
            }else if(cnt == 0 && record[i][j] == ' '){
                cnt++;
                continue;
            }
            
            if(cnt == 1 && record[i][j] != ' '){
                uid += record[i][j];
            }else if(cnt == 1 && record[i][j] == ' '){
                cnt++;
                continue;
            }
            
            if(cnt == 2 && record[i][j] != ' '){
                name += record[i][j];
            }
        }
        
        sub.push_back(action);
        sub.push_back(uid);
        sub.push_back(name);
        v.push_back(sub);
    }
    
    for(int i = 0; i < v.size(); i++){
        if (v[i][0] == "Enter" || v[i][0] == "Change") {
            nick[v[i][1]] = v[i][2];
        }

    }
    
    // for (auto it = nick.begin(); it != nick.end(); ++it) {
    //     cout << (*it).first << ' ' << (*it).second << '\n';
    // }

    for(auto a : v){
        if(a[0] == "Enter"){
            answer.push_back(nick[a[1]]+"님이 들어왔습니다.");
        }
        if(a[0] == "Leave"){
            answer.push_back(nick[a[1]]+"님이 나갔습니다.");
        }
    }
    
    return answer;
}