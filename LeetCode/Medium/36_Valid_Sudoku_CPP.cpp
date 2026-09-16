class Solution {

bool rowcheck(vector<vector<char>>& board, int col){
    set<char> st;
    for(int i = 0; i < 9; i++){
        if(board[i][col] == '.') continue;
        if(st.count(board[i][col]) == 0){
            st.insert(board[i][col]);
        }else{
            cout << i << ' ' << col << '\n';
            return false;
        }
    }

    return true;
}

bool colcheck(vector<vector<char>>& board, int row){
    set<char> st;
    for(int i = 0; i < 9; i++){
        if(board[row][i] == '.') continue;
        if(st.count(board[row][i]) == 0){
            st.insert(board[row][i]);
        }else{
            return false;
        }
    }

    return true;
}

bool squarecheck(vector<vector<char>>& board){
    for(int i = 0; i < 9; i+=3){
        for(int j = 0; j < 9; j+=3){
            set<char> st;
            for(int y = i; y < i + 3; y++){
                for(int x = j; x < j + 3; x++){
                    if(board[y][x] == '.') continue;
                    if(st.count(board[y][x]) == 0){
                        st.insert(board[y][x]);
                    }else{
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

public:
    bool isValidSudoku(vector<vector<char>>& board) {
        bool answer = true;
        bool rowok = true;
        bool colok = true;
        bool squareok = true;
        int n = board.size();

        for(int i = 0; i < n; i++){
            rowok = rowcheck(board, i);
            colok = colcheck(board, i);
            if(!rowok || !colok) return false;
            // cout << colok << ' ';
        }

        squareok = squarecheck(board);
        if(rowok && colok && squareok){
            answer = true;
        }else{
            answer = false;
        }
        return answer;
    }
};