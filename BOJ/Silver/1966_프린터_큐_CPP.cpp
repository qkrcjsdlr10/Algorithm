#include <bits/stdc++.h>
using namespace std;

void func(){
    
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;

    cin >> tc;
    for(int t = 0; t < tc; t++){
        int n = 0;
        int target = 0;
        int cnt = 0;
        int mxPos = 0;
        int mx = 0;
        int weight = 0;
        cin >> n >> target;

        deque<deque<int>> dq(2);

        for(int i = 0; i < n; i++){
            int a = 0;
            cin >> a;
            dq[0].push_back(a);
            dq[1].push_back(i);
            if(i == target){
                weight = a;
            }
        }

        // cout << weight;

        // cout << mx << ' ' << mxPos;

        while(!dq[0].empty()){
            // cout << "adfasdf";
            int sz = dq[0].size();
            mx = 0;

            // cout << mx << ' ' << sz << '\n';
            for(int i = 0; i < sz; i++){
                if(mx < dq[0].front()){
                    mx = dq[0].front();
                    mxPos = i;
                }
                dq[0].push_back(dq[0].front());
                dq[0].pop_front();
            }

            // cout << mx << ' ';

            while(dq[0].front() != mx){
                dq[0].push_back(dq[0].front());
                dq[0].pop_front();
                dq[1].push_back(dq[1].front());
                dq[1].pop_front();
            }
            // cout << mx << ' ' << dq.front() << '\n';

            if(dq[0].front() == weight && dq[1].front() == target){
                break;
            }
            
            dq[0].pop_front();
            dq[1].pop_front();
            cnt++;
        }

        // int sz = dq.size();

        // for(int i = 0; i < sz; i++){
        //     cout << dq.front() << ' ';
        //     dq.pop_front();
        // }

        cout << cnt+1 << '\n';
    }
    

    return 0;
}