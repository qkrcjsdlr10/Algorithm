#include <string>
#include <vector>
#include <set>

using namespace std;

bool solution(vector<string> phone_book) {
    bool answer = true;
    
    set<string> s;
    
    for(int i = 0; i < phone_book.size(); i++){
        s.insert(phone_book[i]);
    }
    
    for(int i = 0; i < phone_book.size(); i++){
        string prefix = "";
        for(int j = 0; j < phone_book[i].size()-1; j++){
            prefix += phone_book[i][j];
            if(s.find(prefix) != s.end()){
                return false;
            }
        }
    }
    
    return true;
}