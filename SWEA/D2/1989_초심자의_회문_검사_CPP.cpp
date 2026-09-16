#include <bits/stdc++.h>
using namespace std;

bool check = false;
string s = "";

void fucn(int start, int end, int size){
    if(end <= start) return;

    if(s[start] == s[end]){
        check = true;
    }else{
        check = false;
        return;
    }

    fucn(start+1, end-1, size);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int tc = 0;
    cin >> tc;
    
    for(int a = 1; a <= tc; a++){
        cin >> s;
        int size = s.size();

        fucn(0, size-1, size);

        cout << '#' << a << ' ' << check << '\n';
    }

    return 0;
}