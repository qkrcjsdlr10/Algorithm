#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int solution(vector<vector<int>> board, vector<int> moves) {
    int answer = 0;
    stack<int> st;
    
    for(auto ar : board){
        for(auto a : ar){
            cout << a;
        }
        cout << '\n';
    }
    
    for(int i = 0; i < moves.size(); i++){
        int idx = moves[i] - 1;
        // cout << idx;
        int target = 0;
        int posy = -1;
        
        for(int j = 0; j < board.size(); j++){
            if(board[j][idx] != 0){
                posy = j;
                break;
            }
        }
        if(posy == -1) continue;
        target = board[posy][idx];
        board[posy][idx] = 0;
        // cout << target << ' ';
        
        if(!st.empty() && st.top() == target){
            answer += 2;
            st.pop();
        }else{
            st.push(target);
        }
    }
    cout << '\n';
    for(auto ar : board){
        for(auto a : ar){
            cout << a;
        }
        cout << '\n';
    }
    return answer;
}