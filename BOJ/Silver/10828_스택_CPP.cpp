#include <bits/stdc++.h>
using namespace std;


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int cnt = 0;
    cin >> cnt;
    stack<int> s;

    for(int i = 0; i<cnt; i++){
        string a;
        cin >> a;

        if(a == "push"){
            int t;
            cin >> t;
            s.push(t);
        }else if(a == "pop"){
            if(s.empty()){
                cout << -1 << '\n';
            }else{
                cout << s.top() << '\n';
                s.pop();
            }
        }else if(a == "size"){
            cout << s.size() << '\n';
        }else if (a == "empty"){
            if(s.empty()){
                cout << 1 << '\n';
            }else{
                cout << 0 << '\n';
            }
        }else if(a == "top"){
            if(s.empty()){
                cout << -1 << '\n';
            }else{
                cout << s.top() << '\n';
            }
        }
    }
}