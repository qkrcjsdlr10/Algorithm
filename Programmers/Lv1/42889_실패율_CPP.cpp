#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace std;

vector<int> solution(int N, vector<int> stages) {
    vector<int> answer;
    int p = stages.size();
    
    vector<int> arr(N, 0);
    vector<double> score(N);
    
    for(int i = 0; i < stages.size(); i++){
        if(stages[i] <= N) {
            arr[stages[i]-1]++;
        }
    }
    
    for(int i = 0; i < N; i++){
        // cout << arr[i] << ' ' << p << ' ';
        score[i] = (double)arr[i] / p;
        p = p - arr[i];
        if(p == 0){
            break;
        }
        // cout << score[i] << ' ';
    }
    cout << '\n';
    // sort(score.rbegin(), score.rend());
    
    for(int i = 0; i < score.size(); i++){
        double mx = -1;
        int idx = -1;
        for(int j = 0; j < score.size(); j++){
            if(score[j] > mx){
                mx = score[j];
                idx = j;
            }
        }
        // cout << idx << ' ';
        score[idx] = -1;
        answer.push_back(idx+1);
    }
    
    return answer;
}