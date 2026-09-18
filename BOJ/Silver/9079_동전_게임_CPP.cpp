#include <bits/stdc++.h>
using namespace std;

vector<vector<int>> maze;
vector<int> vis(512);

void col_change(int col){
    for(int i = 0; i < 3; i++){
        maze[i][col] = (maze[i][col] == 1 ? 0 : 1);
    }
}

void row_change(int row){
    for(int i = 0; i < 3; i++){
        maze[row][i] = (maze[row][i] == 1 ? 0 : 1);
    }
}

void cross_change(int dir){
    for(int i = 0; i < 3; i ++){
        if(dir == 0){
            maze[i][i] = (maze[i][i] == 1 ? 0 : 1);
        }else{
            maze[i][2-i] = (maze[i][2-i] == 1 ? 0 : 1);
        }
    }
}

int maze_int(){
    int now = 0;
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            now = now * 2 + maze[i][j];
        }
    }
    return now;
}

void int_maze(int number){
    for(int i = 2; i >= 0; i--){
        for(int j = 2; j >= 0; j--){
            maze[i][j] = number % 2;
            number /= 2;
        }
    }
}

bool check_maze(){
    int first = maze[0][0];
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            if(first != maze[i][j]){
                return false;
            }
        }
    }
    return true;
}

int bfs(){
    queue<pair<int, int>> q;
    int f = maze_int();
    q.push({f, 0});

    vis[f] = true;
    while(!q.empty()){
        int now = q.front().first;
        int cnt = q.front().second;
        q.pop();
        int_maze(now);

        if(check_maze()){
            return cnt;
        }

        for(int i = 0; i < 3; i++){
            col_change(i);
            int next = maze_int();
            if(!vis[next]){
                vis[next] = true;
                q.push({next, cnt+1});
            }
            col_change(i);
        }

        for(int i = 0; i < 3; i++){
            row_change(i);
            int next = maze_int();
            if(!vis[next]){
                vis[next] = true;
                q.push({next, cnt + 1});
            }
            row_change(i);
        }

        for(int i = 0; i < 2; i++){
            cross_change(i);
            int next = maze_int();
            if(!vis[next]){
                vis[next] = true;
                q.push({next, cnt + 1});
            }
            cross_change(i);
        }

    }
    return -1;
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int t = 0;
    cin >> t;

    while(t--){
        maze.assign(3, vector<int>(3, 0));
        fill(vis.begin(), vis.end(), 0);

        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                char s = ' ';
                cin >> s;
                if(s == 'T'){
                    maze[i][j] = 0;
                }else{
                    maze[i][j] = 1;
                }
            }
        }

        cout << bfs() << '\n';
    }
    

    return 0;
}