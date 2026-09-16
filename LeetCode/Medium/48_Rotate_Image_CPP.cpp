class Solution {

int n = 0;

void rotarr(vector<vector<int>>& matrix){
    vector<vector<int>> temp(n, vector<int> (n));

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            temp[j][n - 1 - i] = matrix[i][j];
        }
    }

    matrix = temp;
}

public:
    void rotate(vector<vector<int>>& matrix) {
        n = matrix.size();
        rotarr(matrix);


    }
};