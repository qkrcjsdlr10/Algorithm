#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    string a = "";
    string b = "";
    int kx = 0;
    int ky = 0;
    int sx = 0;
    int sy = 0;
    vector<string> arr;
    vector<vector<int>> board;
    board.resize(8, vector<int> (8, 0));
    
    int t = 0;

    cin >> a >> b >> t;
    arr.resize(t);
    for(int i = 0; i < t; i++){
        cin >> arr[i];
    }

    // for(int i = 0; i < t; i++){
    //     cout << arr[i] << ' ';
    // }
    // cout << a[0] << a[1] << ' ' << b[0] << b[1] << '\n';
    // cout << int(a[1]) << '\n';

    for(int i = 0; i < 8; i++){
        if(a[0] == 'A'+i){
            kx = i;
        }
        if(int(a[1]) - '0' == i+1){
            ky = 7-i;
        }
        if(b[0] == 'A'+i){
            sx = i;
        }
        if(int(b[1]) - '0' == i+1){
            // cout << "adfasdfasd";
            sy = 7-i;
        }
        // cout << i + 1 << ' ';
        // cout << 'A' << ' ';
    }

    // cout << kx << ky << sx << sy<< '\n';

    board[ky][kx] = 1;
    board[sy][sx] = 2;

    // for(int i = 0; i < 8; i++){
    //     for(int j = 0; j < 8; j++){
    //         cout << board[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }
    // cout << '\n';

    for(int i = 0; i < t; i++){
        if(arr[i] == "L"){
            if(ky == sy && kx - 1 == sx && sx-1 >= 0){
                // cout << "adf";
                board[ky][kx] = 0;
                kx--;
                board[sy][sx] = 1;
                sx--;
                continue;
            }else if(ky == sy && kx - 1 == sx && sx-1 < 0){
                continue;
            }
            if(kx-1 >= 0){
                board[ky][kx] = 0;
                kx--;
            }
        }else if(arr[i] == "R"){
            if(ky == sy && kx + 1 == sx && sx+1 <= 7){
                // cout << "adf";
                board[ky][kx] = 0;
                kx++;
                board[sy][sx] = 1;
                sx++;
                continue;
            }else if(ky == sy && kx + 1 == sx && sx+1 > 7){
                continue;
            }
            if(kx+1 <= 7){
                board[ky][kx] = 0;
                kx++;
            }
        }else if(arr[i] == "T"){
            // cout << "asdfadsfasdf";
            if(kx == sx && ky - 1 == sy && sy-1 >= 0){
                // cout << "adf";
                board[ky][kx] = 0;
                ky--;
                board[sy][sx] = 1;
                sy--;
                continue;
            }else if(kx == sx && ky - 1 == sy && sy-1 < 0){
                continue;
            }
            if(ky-1 >= 0){
                // cout << "adf";
                board[ky][kx] = 0;
                ky--;
            }
            
        }else if(arr[i] == "B"){
            if(kx == sx && ky + 1 == sy && sy+1 <= 7){
                // cout << "adf";
                board[ky][kx] = 0;
                ky++;
                board[sy][sx] = 1;
                sy++;
                continue;
            }else if(kx == sx && ky + 1 == sy && sy+1 > 7){
                continue;
            }
            if(ky+1 <= 7){
                board[ky][kx] = 0;
                ky++;
            }
        }else if(arr[i] == "LT"){
            if(ky-1 == sy && kx - 1 == sx && sx-1 >= 0 && sy-1 >= 0){
                // cout << "adf";
                board[ky][kx] = 0;
                kx--;
                ky--;
                board[sy][sx] = 1;
                sx--;
                sy--;
                continue;
            }else if((ky-1 == sy && kx - 1 == sx) && (sx-1 < 0 || sy-1 < 0)){
                continue;
            }
            if(kx-1 >= 0 && ky-1 >= 0){
                board[ky][kx] = 0;
                kx--;
                ky--;
            }
        }else if(arr[i] == "RT"){
            if(ky-1 == sy && kx + 1 == sx && sx+1 <= 7 && sy-1 >= 0){
                // cout << "adf";
                board[ky][kx] = 0;
                kx++;
                ky--;
                board[sy][sx] = 1;
                sx++;
                sy--;
                continue;
            }else if((ky-1 == sy && kx + 1 == sx) && (sx+1 > 7 || sy-1 < 0)){
                continue;
            }
            if(kx+1 <= 7 && ky-1 >=0){
                board[ky][kx] = 0;
                kx++;
                ky--;
            }
        }else if(arr[i] == "LB"){
            if(ky+1 == sy && kx - 1 == sx && sx-1 >= 0 && sy+1 <= 7){
                // cout << "adf";
                board[ky][kx] = 0;
                kx--;
                ky++;
                board[sy][sx] = 1;
                sx--;
                sy++;
                continue;
            }else if((ky+1 == sy && kx - 1 == sx) && (sx-1 < 0 || sy+1 > 7)){
                continue;
            }
            if(ky+1 <= 7 && kx-1 >= 0){
                board[ky][kx] = 0;
                kx--;
                ky++;
            }
        }else if(arr[i] == "RB"){
            if(ky+1 == sy && kx + 1 == sx && sx+1 <= 7 && sy+1 <= 7){
                // cout << "adf";
                board[ky][kx] = 0;
                kx++;
                ky++;
                board[sy][sx] = 1;
                sx++;
                sy++;
                continue;
            }else if((ky+1 == sy && kx + 1 == sx) && (sx+1 > 7 || sy+1 > 7)){
                continue;
            }
            if(ky+1 <= 7 && kx+1 <= 7){
                board[ky][kx] = 0;
                kx++;
                ky++;
            }
        }
    }
    board[ky][kx] = 1;
    board[sy][sx] = 2;
    // for(int i = 0; i < 8; i++){
    //     for(int j = 0; j < 8; j++){
    //         cout << board[i][j] << ' ';
    //     }
    //     cout << '\n';
    // }
    string resk = "";
    char c = 'A';

    for(int i = 0; i < 8; i++){
        if(i == kx){
            resk += c;
            break;
        }
        c+=1;
    }

    string ress = "";
    c = 'A';
    for(int i = 0; i < 8; i++){
        if(i == sx){
            ress += c;
            break;
        }
        c+=1;
    }
    
    resk += to_string(8-ky);
    ress += to_string(8-sy);
    cout << resk << '\n' << ress;
    return 0;
}