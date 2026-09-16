#include <bits/stdc++.h>
using namespace std;

int n;
int s;


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int testCase = 0;
    cin >> testCase;

    for(int k = 1; k <= testCase; k++){
        cin >> n >> s;
        int cnt = 0;
        int res = 0;
        // n+=2;
        vector<vector<int>> arr(n, vector<int>(n, 0));

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                cin >> arr[i][j];
            }
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(arr[i][j] == 1){
                    cnt++;
                }else {
                    if(cnt == s){
                        res++;
                        cnt = 0;
                        // cout << i << j << ' ';
                    }else{
                        cnt = 0;
                    }
                }
            }
            if(cnt == s){
                // cout << i  << ' ';
                res++;
            }
            cnt = 0;
        }
        
        cnt = 0;

        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                if(arr[j][i] == 1){
                    cnt++;
                }else {
                    if(cnt == s){
                        res++;
                        cnt = 0;
                    }else{
                        cnt = 0;
                    }
                }
            }
            if(cnt == s){
                res++;
            }
            cnt = 0;
        }

        cout << '#' << k << ' ' << res << '\n';
    }

    

    return 0;
}