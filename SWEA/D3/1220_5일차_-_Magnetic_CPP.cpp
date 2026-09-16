#include <bits/stdc++.h>
using namespace std;

int board[105][105];    

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    for(int tc = 1; tc <= 10; tc++){
        cin >> n;
        int cnt = 0;
    
        for(int i = 0; i < 100; i++){
            for(int j = 0; j < 100; j++){
                cin >> board[i][j];
            }
        }
    
        for(int i = 0; i < 100; i++){
            for(int j = 0; j < 100; j++){
                if(board[i][j] == 1){
                    for(int k = i; k < 100; k++){
                        if(board[k][j] == 2){
                            board[k][j] == 0;
                            cnt++;
                            for(int l = i; l <= k; l++){
                                board[l][j] = 0;
                            }
                            break;
                        }
                    }
                }
            }
        }
        
    
        cout << '#' << tc << ' ' << cnt << '\n';
    }



    return 0;
}