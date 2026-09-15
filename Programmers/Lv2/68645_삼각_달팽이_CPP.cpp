#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

vector<vector<int>> arr;

void dfs(int n, int y, int x, int cnt, int depth, int sz){
    // cout << y << ' ' << x << '\n';
    // if(n == 1){
    //     // arr[y+1][x] = cnt++;
    //     return;
    // } 
    if(n <= 0) return;
    arr[y][x] = cnt++;
    for(int i = 1; i < n; i++){
        arr[y+i][x] = cnt++;
    }
    y+=n-1;
    for(int i = 1; i < n; i++){
        arr[y][x+i] = cnt++;
        // x++;
    }
    x+=n-1;
    // y--;
    // cout << y << ' ' << x << '\n';
    
    for(int i = 1; i < n-1; i++){
        arr[y-i][x-i] = cnt++;
    }
//     y+=2;
//     x++;
    // cout << y << ' ' << x << '\n';
    // for(auto ar : arr){
    //     for(auto a : ar){
    //         cout << setw(2) << a << ' ';
    //     }
    //     cout << '\n';
    // }
    // cout << '\n';
    dfs(n-3, depth*2, depth, cnt, depth+1, sz);
}

vector<int> solution(int n) {
    vector<int> answer;
    // n = 3;
    arr.resize(n, vector<int> (n));
    
    dfs(n, 0, 0, 1, 1, n);
    
    for(auto ar : arr){
        for(auto a : ar){
            // cout << setw(2) << a << ' ';
            if(a != 0) answer.push_back(a);
        }
        // cout << '\n';
    }
    
    return answer;
}