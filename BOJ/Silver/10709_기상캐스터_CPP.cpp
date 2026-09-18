#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int m = 0;
    vector<vector<char>> arr;
    vector<vector<int>> answer;
    cin >> n >> m;
    
    arr.resize(n, vector<char> (m));
    answer.resize(n, vector<int> (m));

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> arr[i][j];
        }
    }
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(arr[i][j] == 'c'){
                answer[i][j] = 0;
            }else{
                answer[i][j] = -1;
            }
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m-1; j++){
            if(answer[i][j] >= 0 && answer[i][j+1] == -1){
                answer[i][j+1] = answer[i][j] + 1;
            }
        }
    }

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cout << answer[i][j] << ' ';
        }
        cout << '\n';
    }

    return 0;
}