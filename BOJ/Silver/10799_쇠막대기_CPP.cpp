#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    stack<char> s;
    int a = 0;


    string str;
    cin >> str;
    for(int i = 0; i < str.size(); i++){
        if(str[i] == '('){
            s.push(str[i]);
        }else{
            if(str[i-1] == '('){
                s.pop();
                a = a + s.size();
            }else{
                s.pop();
                a++;
            }
        }
    } 

    cout << a;

}