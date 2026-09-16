#include <bits/stdc++.h>
using namespace std;



int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int testCase = 0;
    int t = 0;
    cin >> testCase;

    for(int tc = 1; tc <= testCase; tc++){
        int score[105] = {0};
        cin >> t;
        int maxScore = 0;
        int maxPos = 0;
    
        for(int i = 0; i < 1000; i++){
            int num = 0;
            cin >> num;
            score[num]++;
        }
    
        for(int i = 0; i <= 100; i++){
            if(score[i] >= maxScore){
                maxScore = score[i];
                maxPos = i;
            }
        }
    
        cout << '#' << tc << ' ' << maxPos << '\n';
    }

    return 0;
}