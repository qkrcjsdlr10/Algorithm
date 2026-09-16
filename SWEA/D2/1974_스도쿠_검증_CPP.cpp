#include <bits/stdc++.h>
using namespace std;

int arr[10][10];
bool check = true;

void rowCheck(){
    for(int i = 0; i < 9; i++){
        int sdc[10] = {0};
        for(int j = 0; j < 9; j++){
            sdc[arr[i][j]]++;
        }

        for(int j = 1; j <= 9; j++){
            if(sdc[j] != 1){
                check = false;
                return;
            }
        }
    }
}

void colCheck(){
    for(int i = 0; i < 9; i++){
        int sdc[10] = {0};
        for(int j = 0; j < 9; j++){
            sdc[arr[j][i]]++;
        }

        for(int j = 1; j <= 9; j++){
            if(sdc[j] != 1){
                check = false;
                return;
            }
        }
    }
}

void squarCheck(){
    for(int i = 0; i < 9; i+=3){
        for(int j = 0; j < 9; j+=3){
            int sdc[10] = {0};
            for(int k = i; k < i+3; k++){
                for(int l = j; l < j+3; l++){
                    sdc[arr[k][l]]++;
                }
            }
            for(int c = 1; c <= 9; c++){
                if(sdc[c] != 1){
                    check = false;
                    return;
                }
            }
        }
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 0; t < 10; t++){
        check = true;
        for(int i = 0; i < 9; i++){
            for(int j = 0; j < 9; j++){
                cin >> arr[i][j];
            }
        }

        rowCheck();
        colCheck();
        squarCheck();
        
        cout << '#' << t+1 << ' ' << check << '\n';
    }
    



    return 0;
}