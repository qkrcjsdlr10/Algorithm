#include <string>
#include <vector>
#include <iostream>
#include <set>

using namespace std;

vector<int> solution(int n, vector<string> words) {
    vector<int> answer;
    string check = words[0];
    
    set<string> st;
    st.insert(check);
    
    for(int i = 1; i < words.size(); i++){
        check = words[i-1];
        if(check[check.size()-1] != words[i][0]){
            if((i+1) % n == 0){
                answer.push_back(n);
            }else{
                answer.push_back(i % n + 1);
            }
            answer.push_back((i) / n + 1);
            break;
        }
        
        if(st.find(words[i]) == st.end()){
            st.insert(words[i]);
        }else{
            cout << i;
            if((i+1) % n == 0){
                answer.push_back(n);
            }else{
                answer.push_back(i % n + 1);
            }
            
            answer.push_back((i) / n + 1);
            break;
        }
    }
    
    if(answer.size() == 0){
        answer.push_back(0);
        answer.push_back(0);
    }

    return answer;
}