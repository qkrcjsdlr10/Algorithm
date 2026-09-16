class Solution {

int dx[8] = {0, 1, 1, 1, 0, -1, -1, -1};
int dy[8] = {-1, -1, 0, 1, 1, 1, 0, -1};

bool areacheck(int n, int m, int y, int x, int d){
    int ny = y + dy[d];
    int nx = x + dx[d];
    if(ny >= n || ny < 0 || nx >= m || nx < 0) return false;
    return true;
}

public:
    void gameOfLife(vector<vector<int>>& board) {
        int n = board.size();
        int m = board[0].size();

        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                int live = 0;
                for(int k = 0; k < 8; k++){
                    if(areacheck(n, m, i, j, k)){
                        int nx = j + dx[k];
                        int ny = i + dy[k];
                        if(board[ny][nx] == 1 || board[ny][nx] == -1) live++;
                    }
                }

                if(board[i][j] == 0){
                    if(live == 3){
                        board[i][j] = 2;
                    }
                }else{
                    if(live > 3){
                        board[i][j] = -1;
                    }else if(live < 2){
                        board[i][j] = -1;
                    }else if(live == 2 || live == 3){
                        continue;
                    }
                }
                
            }
        }

        for(int i = 0; i < n; i++){
            for(int j = 0; j < m; j++){
                if(board[i][j] == -1){
                    board[i][j] = 0;
                }
                if(board[i][j] == 2){
                    board[i][j] = 1;
                }
            }
        }

    }
};