#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int testcase = 0;
    cin >> testcase;

    for(int test = 0; test < testcase; test++){
        int tc = 0;
        cin >> tc;
        int mx = 0;
        int mxscore = 0;
        int res = 0;
        vector<int> arr;
        vector<int> same;
        vector<int> answer;
        vector<int> resscore;
        vector<vector<int>> rscore;
        arr.resize(tc);

        for(int i = 0; i < tc; i++){
            cin >> arr[i];
            mx = max(arr[i], mx);
        }

        vector<vector<int>> score;
        score.resize(mx+1, vector<int> (2));
        rscore.resize(mx+1, vector<int> (2));
        for(int i = 0; i < tc; i++){
            score[arr[i]][0]++;
            // if(score[arr[i]][0] <= 4){
            //     score[arr[i]][1] += i+1;
            // }
        }

        for (int i = 0; i < tc; i++) {
            if (score[arr[i]][0] == 6) resscore.push_back(arr[i]);
        }


        for(int i = 0; i < resscore.size(); i++){
            if(rscore[resscore[i]][0] < 4){
                rscore[resscore[i]][0]++;
                rscore[resscore[i]][1] += i+1;
                // cout << resscore[i] << ' ';
            }
            // cout << resscore[i] << ' ';
        }
        // cout << '\n';

        // for(int i = 1; i < mx + 1; i++){
        //     cout << rscore[i][0] << ' ' << rscore[i][1] << '\n';
        // }

        int cnt = 0;

        int mnscore = 1e9;
        for (int i = 1; i <= mx; i++) {
            if (rscore[i][0] == 4) {
                mnscore = min(mnscore, rscore[i][1]);
            }
        }
        
        for (int i = 1; i <= mx; i++) {
            if (rscore[i][0] == 4 && rscore[i][1] == mnscore) {
                same.push_back(i);
            }
        }

        if(same.size() == 0){
            for(int i = 1; i < mx+1; i++){
                if(rscore[i][0] == 4){
                    if(mxscore == rscore[i][1]){
                        cout << i;
                        break;
                    }
                    
                }
            }
        }else{
            vector<int> asdf;
            asdf.resize(mx+1);
            for(int i = 0; i < resscore.size(); i++){
                asdf[resscore[i]]++;
                if(asdf[resscore[i]] == 5 && rscore[resscore[i]][1] == mnscore){
                    res = resscore[i];
                    break;
                }
            }
            cout << res << '\n';
        }
    }

    

    return 0;
}