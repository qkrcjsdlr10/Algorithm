#include <string>
#include <vector>
#include <bits/stdc++.h>


using namespace std;

vector<int> solution(vector<int> progresses, vector<int> speeds) {
    vector<int> answer;
    
    vector<int> arr;
    int cnt = 0;
    
    for(int i = 0; i < progresses.size(); i++){
        int per = 100 - progresses[i];
        int a = (per + speeds[i]-1) / speeds[i];
        arr.push_back(a);
    }
    int idx = 0;
    while(idx < arr.size()){
        int deployDay = arr[idx];
        int cnt = 1;
        int j = idx + 1;

        while (j < (int)arr.size() && arr[j] <= deployDay) {
            cnt++;
            j++;
        }

        answer.push_back(cnt);
        idx = j;
    }
    
    return answer;
}