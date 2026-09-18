#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int board[5][5] = {};
    int arr[25] = {};
    bool vis[5][5] = {false};
    int cnt = 0;
    int bingo = 0;

    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            cin >> board[i][j];
        }
    }

    for(int i = 0; i < 25; i++){
        cin >> arr[i];
    }

    for(int i = 0; i < 25; i++){
        bingo = 0;
        for(int k = 0; k < 5; k++){
            for(int l = 0; l < 5; l++){
                if(board[k][l] == arr[i]){
                    vis[k][l] = true;
                    cnt++;
                }
            }
        }
        
        for(int k = 0; k < 5; k++){
            int check = 0;
            for(int l = 0; l < 5; l++){
                if(vis[k][l] == true){
                    check++;
                }
            }
            if(check == 5){
                bingo++;
            }
        }

        for(int k = 0; k < 5; k++){
            int check = 0;
            for(int l = 0; l < 5; l++){
                if(vis[l][k] == true){
                    check++;
                }
            }
            if(check == 5){
                bingo++;
            }
        }
        
        int check = 0;
        for(int k = 0; k < 5; k++){
            if(vis[k][k] == true){
                check++;
            }
            
        }
        if(check == 5){
            bingo++;
        }
        
        check = 0;
        for(int k = 0; k < 5; k++){
            if(vis[k][4-k] == true){
                check++;
            }
            
        }
        if(check == 5){
            bingo++;
        }
        if(bingo >= 3) break;
    }

    cout << cnt << '\n';

    return 0;
}