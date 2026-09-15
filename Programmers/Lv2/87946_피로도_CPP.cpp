#include <string>
#include <vector>
#include <iostream>

using namespace std;

int ed = 0;
int mx = 0;
int res = 0;
vector<bool> vis;

void dfs(int hp, vector<vector<int>> dungeons, int depth){
//     if(hp <= 0) return;
    
//     if(depth == ed){
//         if(hp > mx){
//             mx = hp;
//         }
//         int temp = 0;
//         for(int i = 0; i < ed; i++){
//             if(vis[i]){
//                 temp++;
//             }
//         }
//         res = max(temp, res);
//         return;
//     }
    
    res = max(res, depth);
    
    for(int i = 0; i < ed; i++){
        if(vis[i]) continue;
        if(hp < dungeons[i][0]) continue;
        if(hp - dungeons[i][1] < 0) continue;
        vis[i] = 1;
        dfs(hp - dungeons[i][1], dungeons, depth + 1);
        vis[i] = 0;
    }
}

int solution(int k, vector<vector<int>> dungeons) {
    int answer = -1;
    
    ed = dungeons.size();
    vis.resize(ed);
    
    dfs(k, dungeons, 0);
    
    cout << mx << ' ' << res;
    answer = res;
    return answer;
}