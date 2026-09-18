#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    cin >> n;
    int time = 0;
    int score = 0;
    time = n;

    stack<pair<int, int>> st;

    for(int i = 0; i < n; i++){
        int a = 0;
        int b = 0;
        int c = 0;
        cin >> a;

        if(a == 0){
            if(!st.empty()){
                st.top().second--;
                if(st.top().second == 0){
                    score += st.top().first;
                    st.pop();
                }
            }
        }else{
            cin >> b >> c;
            // st.push({b, c-1});
            if(c == 1){
                score += b;
            }else{
                c--;
                st.push({b, c});
            }
        }
    }
    

    cout << score;

    return 0;
}