#include <bits/stdc++.h>
using namespace std;

int main(){
    int n;
    cin >> n;
    deque<int> dq;

    while(n--){
        string s;
        cin >> s;

        if(s == "push_front"){
            int i;
            cin >> i;
            dq.push_front(i);
        }else if(s == "push_back"){
            int i;
            cin >> i;
            dq.push_back(i);
        }else if(s == "pop_front"){
            if(dq.empty()){
                cout << -1 << '\n';
            }else{
                cout << dq.front() << '\n';
                dq.pop_front();
            }
        }else if(s == "pop_back"){
            if(dq.empty()){
                cout << -1 << '\n';
            }else{
                cout << dq.back() << '\n';
                dq.pop_back();
            }
        }else if(s == "size"){
            cout << dq.size() << '\n';
        }else if(s == "empty"){
            if(dq.empty()){
                cout << 1 << '\n';
            }else{
                cout << 0 << '\n';
            }
            
        }else if(s == "front"){
            if(dq.empty()){
                cout << -1 << '\n';
            }else{
                cout << dq.front() << '\n';
            }
            
        }else if(s == "back"){
            if(dq.empty()){
                cout << -1 << '\n';
            }else{
                cout << dq.back() << '\n';
            }
            
        }
    }
}