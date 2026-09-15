#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

vector<int> solution(vector<int> lottos, vector<int> win_nums) {
    int arr[7] = {6, 6, 5, 4, 3, 2, 1};
    vector<int> answer;
    int cnt = 0;
    int n = 0;
    for(int i = 0; i < lottos.size(); i++){
        if(lottos[i] == 0){
            cnt++;
            n++;
            continue;
        }
        auto it = find(win_nums.begin(), win_nums.end(), lottos[i]);
        if(it != win_nums.end()) cnt++;
    }
    cout << cnt << ' ' << cnt - n;
    answer.push_back(arr[cnt]);
    answer.push_back(arr[cnt-n]);
     
    return answer;
}