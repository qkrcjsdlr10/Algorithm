#include <string>
#include <vector>

using namespace std;

string solution(vector<string> cards1, vector<string> cards2, vector<string> goal) {
    string answer = "";
    int c1 = 0, c2 = 0;
    bool check = true;
    
    for(int i = 0; i < goal.size(); i++){
        int a = 0;
        int b = 0;
        if(goal[i] == cards1[c1]){
            c1++;
            a++;
        }
        
        if(goal[i] == cards2[c2]){
            c2++;
            b++;
        }
        
        if(a + b != 1){
            check = false;
            break;
        }
    }
    
    if(check == true){
        answer = "Yes";
    }else{
        answer = "No";
    }
    
    return answer;
}