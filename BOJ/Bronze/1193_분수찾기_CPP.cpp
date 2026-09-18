#include <bits/stdc++.h>
using namespace std;

int n;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;
    int line = 1;
    int x, y = 0;

    while(n > line){
        n -= line;
        line++;
    }

    if(line % 2 == 0){
        y = n;
        x = line - n + 1;
    }else{
        y = line - n + 1;
        x = n;
    }
    
    cout << y << "/" << x;

    return 0;
}