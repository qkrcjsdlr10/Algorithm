#include <string>
#include <vector>
#include <bits/stdc++.h>


using namespace std;

int solution(vector<string> friends, vector<string> gifts) {
    int answer = 0;
    
    unordered_map<string, unordered_map<string, int>> m;
    
    unordered_map<string, int> give;
    unordered_map<string, int> receive;
    unordered_map<string, int> score;
    unordered_map<string, int> givenext;
    
    
    for(auto gift : gifts){
        stringstream ss(gift);
        string from, to;
        getline(ss, from, ' ');
        getline(ss, to, ' ');
        give[from]++;
        receive[to]++;
        m[from][to]++;
    }
    
    for(auto name : friends){
        score[name] = give[name] - receive[name];
    }
    
    for(int i = 0; i < friends.size(); i++){
        for(int j = i + 1; j < friends.size(); j++){
            if(i == j) continue;
            
            string a = friends[i];
            string b = friends[j];
            
            int ab = m[a][b];
            int ba = m[b][a];
            
            if(ab > ba){
                givenext[a]++;
            }else if(ba > ab){
                givenext[b]++;
            }else{
                if(score[a] > score[b]){
                    givenext[a]++;
                }else if(score[b] > score[a]){
                    givenext[b]++;
                }
            }
        }
    }
    
    for(auto name : friends){
        answer = max(answer, givenext[name]);
    }
    
    return answer;
}