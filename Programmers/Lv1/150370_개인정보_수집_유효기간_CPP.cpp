#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(string today, vector<string> terms, vector<string> privacies) {
    vector<int> answer;
    int nowy = stoi(today.substr(0, 4));
    int nowm = stoi(today.substr(5, 2));
    int nowd = stoi(today.substr(8, 2));
    cout << nowy << ' ' << nowm << ' ' << nowd << '\n';
    
    for(int i = 0; i < privacies.size(); i++){
        int month = 0;
        int year = 0;
        int day = 0;
        int addmonth = 0;
        for(int j = 0; j < terms.size(); j++){
            if(privacies[i].back() == terms[j][0]){
                addmonth = stoi(terms[j].substr(2));
            }
        }
        month = stoi(privacies[i].substr(5, 2));
        year = stoi(privacies[i].substr(0, 4));
        day = stoi(privacies[i].substr(8, 2));
        
        month += addmonth;
        
        year += (month - 1) / 12;
        month = (month - 1) % 12 + 1;
        
        day--;
        if(day == 0){
            month--;
            day = 28;
            if(month == 0){
                month = 12;
                year--;
            }
        }
        
        cout << year << ' ' << month << ' ' << day << '\n';
        
        
        if(nowy > year){
            answer.push_back(i+1);
            continue;
        }else if(nowy < year){
            continue;
        }
        
        if(nowm > month){
            answer.push_back(i+1);
            continue;
        }else if(nowm < month){
            continue;
        }
        
        if(nowd > day){
            answer.push_back(i+1);
            continue;
        }
    }
    
    return answer;
}