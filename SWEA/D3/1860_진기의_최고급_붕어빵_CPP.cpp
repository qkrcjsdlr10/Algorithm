#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int tc = 0;
    cin >> tc;
    for(int t = 1; t <= tc; t++){
        bool res = 1;
        int before = 0;
        int n = 0, m = 0, k = 0;
        cin >> n >> m >> k;
        int mx = 0;
        int produced = 0;
        int sold  = 0;

        vector<int> time(11111);

        for(int i = 0; i < n; i++){
            int num = 0; 
            cin >> num;
            time[num]++;
            mx = max(mx, num);
        }

        for(int i = 0; i <= mx; i++){
            if(!time[i]) continue;

            produced = (i / m) * k;
            sold += time[i];
            
            if(produced < sold){
                res = false;
                break;
            }
        }
        cout << '#' << t << " ";

        cout << (res ?  "Possible" :  "Impossible") << '\n';
    }
    

    return 0;
}