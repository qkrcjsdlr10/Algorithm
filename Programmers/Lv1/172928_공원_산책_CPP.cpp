#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};

int dircheck(char c){
    if(c == 'N') return 0;
    if(c == 'E') return 1;
    if(c == 'S') return 2;
    if(c == 'W') return 3;
}

vector<int> solution(vector<string> park, vector<string> routes) {
    vector<int> answer;
    int n = park.size();
    int m = park[0].size();
    int curx = 0;
    int cury = 0;
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(park[i][j] == 'S'){
                curx = j;
                cury = i;
            }
        }
    }
    
    for(auto str : routes){
        stringstream ss(str);
        bool canmove = true;
        char dir = 0;
        int dis = 0;
        ss >> dir >> dis;
        // cout << dir << ' ' << dis << '\n';
        
        int d = dircheck(dir);
        // cout << d;
        int x = curx;
        int y = cury;
        
        for(int i = 0; i < dis; i++){
            int nx = x + dx[d];
            int ny = y + dy[d];
            // cout << ny << ' ' << nx << '\n';
            if(nx < 0 || nx >= m || ny < 0 || ny >= n){
                canmove = false;
                break;
            } 
            if(park[ny][nx] == 'X'){
                canmove = false;
                break;
            }
            x += dx[d];
            y += dy[d];
            // cout << y << ' ' << x << '\n';
        }
        
        if(canmove){
            park[cury][curx] = 'O';
            if(d == 0){
                cury -= dis;
            }else if(d == 1){
                curx += dis;
            }else if(d == 2){
                cury += dis;
            }else if(d == 3){
                curx -= dis;
            }
            park[cury][curx] = 'S';
        }
        // cout << cury << ' ' << curx << '\n';
        for(auto pa : park){
            for(auto p : pa){
                cout << p << ' ';
            }
            cout << '\n';
        }
        cout << '\n';
    }
    
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            if(park[i][j] == 'S'){
                answer.push_back(i);
                answer.push_back(j);
                break;
            }
        }
    }
    
    return answer;
}