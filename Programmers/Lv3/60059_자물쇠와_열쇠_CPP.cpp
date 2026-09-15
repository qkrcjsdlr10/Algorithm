#include <string>
#include <vector>

using namespace std;

void rotkey(vector<vector<int>> &key){
    int m = key.size();
    vector<vector<int>> temp(m, vector<int>(m));

    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            temp[j][m - 1 - i] = key[i][j];
        }
    }

    key = temp;
}

bool check(vector<vector<int>> &key, vector<vector<int>> &lock, int y, int x, int holeCnt){
    int n = lock.size();
    int m = key.size();

    int filled = 0;

    for(int i = 0; i < m; i++){
        for(int j = 0; j < m; j++){
            int ly = y + i;
            int lx = x + j;

            if(ly < 0 || ly >= n || lx < 0 || lx >= n) continue;

            if(lock[ly][lx] == 1 && key[i][j] == 1){
                return false;
            }

            if(lock[ly][lx] == 0 && key[i][j] == 1){
                filled++;
            }
        }
    }

    return filled == holeCnt;
}

bool solution(vector<vector<int>> key, vector<vector<int>> lock) {
    bool answer = false;

    int n = lock.size();
    int m = key.size();
    int holeCnt = 0;

    for(auto row : lock){
        for(auto value : row){
            if(value == 0){
                holeCnt++;
            }
        }
    }

    int cnt = 4;

    while(cnt--){
        for(int y = -m + 1; y < n; y++){
            for(int x = -m + 1; x < n; x++){
                if(check(key, lock, y, x, holeCnt)){
                    return true;
                }
            }
        }

        rotkey(key);
    }

    return answer;
}