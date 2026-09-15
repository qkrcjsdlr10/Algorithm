#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<vector<bool>> check;

bool blockCheck(int y, int x, const vector<string>& board){
    if (board[y][x] == ' ') return false;
    if(board[y][x] == board[y+1][x] &&
      board[y][x] == board[y][x+1] && 
      board[y][x] == board[y+1][x+1]){
        return true;
    }else{
        return false;
    }
}

void editCheck(int y, int x){
    check[y][x] = 1;
    check[y+1][x] = 1;
    check[y][x+1] = 1;
    check[y+1][x+1] = 1;
}

void deleteBoard(int y, int x, vector<string> &board){
    board[y][x] = ' ';
}


int solution(int m, int n, vector<string> board) {
    int answer = 0;
    
    int cnt = 1;
    int a = 0;
    
    check.resize(board.size(), vector<bool>(board[0].size()));
    
    while(cnt != 0){
        cnt = 0;
        check.assign(m, vector<bool>(n, false));
        
        for(int i = 0; i < board.size()-1; i++){
            for(int j = 0; j < board[0].size()-1; j++){
                bool c = blockCheck(i, j, board);
                if(c){
                    editCheck(i, j);
                    cnt++;
                } 
            }
        }
        
        for(int i = 0; i < board.size(); i++){
            for(int j = 0; j < board[0].size(); j++){
                if(check[i][j]) deleteBoard(i, j, board);
            }
        }
    
        for(int i = board.size()-2; i >= 0 ; i--){
            for(int j = board[0].size()-1; j >= 0 ; j--){
                int ny = i;
                if(board[i][j] != ' ' && board[i+1][j] == ' '){
                    for(int k = i+1; k < board.size(); k++){
                        if(board[k][j] == ' '){
                            // board[k][j] = ' ';
                            ny++;
                        }else{
                            break;
                        }
                    }
                    board[ny][j] = board[i][j];
                    board[i][j] = ' ';
                } 
            }
        }
        // for(auto c : check){
        //     for(auto cc : c){
        //         if(cc) a++;
        //         // cout << cc << ' ';
        //     }
        //     // cout << '\n';
        // }
    }
    
    
    
    for(auto c : board){
        for(auto cc : c){
            if(cc == ' ') a++;
            cout << cc << ' ';
        }
        cout << '\n';
    }
    cout << '\n';
    // for(auto c : check){
    //     for(auto cc : c){
    //         if(cc) a++;
    //         cout << cc << ' ';
    //     }
    //     cout << '\n';
    // }
    cout << a;
    answer = a;
    return answer;
}