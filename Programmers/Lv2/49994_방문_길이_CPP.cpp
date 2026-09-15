#include <string>
#include <vector>
#include <set>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int solution(string dirs) {
    int answer = 0;
    int idx = 0;
    int curx = 5;
    int cury = 5;
    set<vector<int>> st;
    
    vector<vector<int>> board;
    vector<vector<bool>> vis;
    
    board.resize(11, vector<int> (11));
    vis.resize(11, vector<bool> (11));
    
    while(idx < dirs.size()){
        char dir = dirs[idx];
        vector<int> arr;
        arr.resize(4);
        int d = -1;
        int nx = 0;
        int ny = 0;
        
        if(dir == 'U'){
            d = 0;
            nx += dx[d];
            ny += dy[d];
        }else if(dir == 'R'){
            d = 1;
            nx += dx[d];
            ny += dy[d];
        }else if(dir == 'D'){
            d = 2;
            nx += dx[d];
            ny += dy[d];
        }else if(dir == 'L'){
            d = 3;
            nx += dx[d];
            ny += dy[d];
        }
        
        idx++;
        
        if(curx + nx < 0 || curx + nx > 10) continue;
        if(cury + ny < 0 || cury + ny > 10) continue;
        arr[0] = cury;
        arr[1] = curx;
        curx += nx;
        cury += ny;
        if(arr[0] < cury || (arr[0] == cury && arr[1] < curx)){
            arr[2] = cury;
            arr[3] = curx;
        }else{
            arr[2] = arr[0];
            arr[3] = arr[1];
            arr[0] = cury;
            arr[1] = curx;
        }
        
        if(st.find(arr) == st.end()){
            answer++;
            st.insert(arr);
        }
        // if(vis[cury][curx] == false){
        //     answer++;
        //     vis[cury][curx] = true;
        // }
    }
    
    return answer;
}