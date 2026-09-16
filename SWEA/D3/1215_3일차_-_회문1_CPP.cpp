#include <bits/stdc++.h>
using namespace std;

char board[10][10];
int res;
int n;
bool check;

void fucn(string s, int start, int end){
    if(start >= end){
        // if(check == true){
            res++;
        // }
        return;
    }

    // cout << s[start] << s[end] << '\n';

    if(s[start] != s[end]){
        return;
    }
    check = true;

    fucn(s, start+1, end-1);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    string str[8];

    for(int a = 1; a <= 10; a++){
        cin >> n;
        cin.ignore();

        for(int i = 0; i < 8; i++){
            getline(cin, str[i]);
            // cin >> str[i];
        }


        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                board[i][j] = str[i][j];
            }
        }

        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(j+n > 8){
                    continue;
                }

                string s = "";

                for(int k = j; k < j+n; k++){
                    s += board[i][k];
                }
                check = false;
                fucn(s, 0, n-1);
                
            }
        }

        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                if(j+n > 8){
                    continue;
                }

                string s = "";

                for(int k = j; k < j+n; k++){
                    s += board[k][i];
                }
                check = false;
                fucn(s, 0, n-1);
            }
        }

        cout << '#' << a << ' ' << res << '\n';
        res = 0;
    }

    return 0;
}