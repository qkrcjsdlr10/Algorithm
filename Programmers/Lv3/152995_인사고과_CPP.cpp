#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// 근무태도 내림차순, 근무태도 같으면 동료평가 오름차순
bool cmp(const vector<int>& a, const vector<int>& b){
    if(a[0] != b[0]) return a[0] > b[0];
    return a[1] < b[1];
}

int solution(vector<vector<int>> scores) {
    int n = scores.size();
    vector<int> wanho = scores[0];
    int wanhoSum = wanho[0] + wanho[1];

    sort(scores.begin(), scores.end(), cmp);

    int maxPeer = 0;
    bool wanhoFail = false;
    vector<bool> alive(n, true);

    // 스위핑: 탈락자 체크
    for(int i = 0; i < n; i++){
        if(scores[i][1] < maxPeer){
            alive[i] = false;
            if(scores[i] == wanho) wanhoFail = true;
        } else {
            maxPeer = scores[i][1];
        }
    }

    if(wanhoFail) return -1;

    // 순위 계산
    int rank = 1;
    for(int i = 0; i < n; i++){
        if(!alive[i] || scores[i] == wanho) continue;
        if(scores[i][0] + scores[i][1] > wanhoSum) rank++;
    }

    return rank;
}