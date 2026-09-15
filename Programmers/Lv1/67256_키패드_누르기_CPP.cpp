#include <string>
#include <vector>
#include <iostream>

using namespace std;

string solution(vector<int> numbers, string hand) {
    string answer = "";
    int l = 0;
    int r = 0;
    int lcurx = 0;
    int lcury = 3;
    int rcurx = 2;
    int rcury = 3;
    int cnt = 1;
    
    vector<vector<int>> board(4, vector<int> (3));
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            board[i][j] = cnt++;
        }
    }
    
    board[3][0] = -1;
    board[3][2] = -1;
    
    // for(auto ar : board){
    //     for(auto a : ar){
    //         cout << a;
    //     }
    //     cout << '\n';
    // }
    for(int i = 0; i < numbers.size(); i++){
        int n = numbers[i];
        cout << lcury << lcurx << ' ' << rcury << rcurx << '\n';;
        if(n == 1 || n == 4 || n == 7){
            for(int j = 0; j < 3; j++){
                if(board[j][0] == n){
                    lcurx = 0;
                    lcury = j;
                    break;
                }
            }
            // cout << lcurx << lcury << '\n';
            answer.push_back('L');
            continue;
        } 
        if(n == 3 || n == 6 || n == 9){
            for(int j = 0; j < 3; j++){
                if(board[j][2] == n){
                    rcurx = 2;
                    rcury = j;
                    break;
                }
            }
            
            answer.push_back('R');
            continue;
        } 
        for(int j = 0; j < 4; j++){
            if(n == board[j][1]){
                int ld = 0;
                int rd = 0;
                ld = abs(1 - lcurx) + abs(j - lcury);
                rd = abs(1 - rcurx) + abs(j - rcury);
                // cout << ld << ' ' << rd << '\n';
                if(ld > rd){
                    answer.push_back('R');
                    rcurx = 1;
                    rcury = j;
                }else if(ld == rd){
                    if(toupper(hand[0]) == 'R'){
                        rcurx = 1;
                        rcury = j;
                    }else{
                        lcurx = 1;
                        lcury = j;
                    }
                    answer.push_back(toupper(hand[0]));
                }else{
                    answer.push_back('L');
                    lcurx = 1;
                    lcury = j;
                }
                break;
            }
        }
    }
    
    return answer;
}