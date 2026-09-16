class Solution {

int rows = 0;
int cols = 0;
int k = 0;
vector<int> answer;

void snail(int x, int y, int depth, vector<vector<int>>& matrix){
    int h = rows - depth * 2;
    int w = cols - depth * 2;

    if (h <= 0 || w <= 0) return;

    // 한 줄만 남은 경우
    if (h == 1) {
        for (int i = 0; i < w; i++) {
            answer.push_back(matrix[y][x + i]);
        }
        return;
    }

    // 한 열만 남은 경우
    if (w == 1) {
        for (int i = 0; i < h; i++) {
            answer.push_back(matrix[y + i][x]);
        }
        return;
    }

    for(int i = 0; i < w; i++){
        answer.push_back(matrix[y][x + i]);
    }

    x += w - 1;
    y++;

    for(int i = 0; i < h - 1; i++){
        answer.push_back(matrix[y + i][x]);
    }

    x--;
    y += h - 2;

    for(int i = 0; i < w - 1; i++){
        answer.push_back(matrix[y][x - i]);
    }

    x -= w - 2;
    y--;

    for(int i = 0; i < h - 2; i++){
        answer.push_back(matrix[y - i][x]);
    }

    snail(depth + 1, depth + 1, depth + 1, matrix);
}

public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        
        rows = matrix.size();
        cols = matrix[0].size();
        k = rows;
        if(rows == 1) return matrix[0];
        if(cols == 1){
            for(auto ar : matrix){
                for(auto a : ar){
                    answer.push_back(a);
                }
            }
            return answer;
        }
        snail(0, 0, 0, matrix);

        // for(auto temp : answer){
        //     cout << temp;
        // }

        return answer;
    }
};

// [1,2,3,4],
// [5,6,7,8],
// [9,10,11,12],
// [13,14,15,16],
// [17,18,19,20],
// [21,22,23,24]