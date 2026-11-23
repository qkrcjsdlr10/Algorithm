import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class Solution {

    static int[] dx = {1, -1, -1, 1};
    static int[] dy = {1, 1, -1, -1};
    static int n;
    static int[][] board;

    static int curX;
    static int curY;
    static int res;

    public static void moveDown(int y, int x, int d,  int rCnt, int lCnt, List<Integer> arr){
        int nx = x + dx[d];
        int ny = y + dy[d];

        if(nx >= n || nx < 0 || ny >= n || ny < 0) return;
        if(d == 0) rCnt++;
        if(d == 1) lCnt++;

        if(arr.contains(board[ny][nx])) return;
        arr.add(board[ny][nx]);

        if(d == 0){
            moveDown(ny, nx, 1, rCnt, lCnt, arr);
            moveDown(ny, nx, 0, rCnt, lCnt, arr);
        }
        if(d == 1){
            moveDown(ny, nx, 1, rCnt, lCnt, arr);
            moveUp(ny, nx, 2, rCnt, lCnt, arr);
        }
        arr.remove(arr.size() - 1);
    }

    public static void moveUp(int y, int x, int d,  int rCnt, int lCnt, List<Integer> arr){
        if(d == 2){
            if(rCnt == 0){
                moveUp(y, x, 3, rCnt, lCnt, arr);
                return;
            }
        }

        int nx = x + dx[d];
        int ny = y + dy[d];

        if(nx >= n || nx < 0 || ny >= n || ny < 0) return;
        if(d == 2) rCnt--;
        if(d == 3) lCnt--;

        if(d == 3 && lCnt == 0){
            if(ny == curY && nx == curX){
                res = Math.max(res, arr.size());
            }
            return;
        }

        if(arr.contains(board[ny][nx])) return;
        arr.add(board[ny][nx]);

        if(d == 2){
            moveUp(ny, nx, 2, rCnt, lCnt, arr);
        }
        if(d == 3){
            moveUp(ny, nx, 3, rCnt, lCnt, arr);
        }
        arr.remove(arr.size() - 1);
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuffer sb = new StringBuffer();
        int tc = Integer.parseInt(br.readLine());
        for(int t = 1; t <= tc; t++){
            res = 0;
            n = Integer.parseInt(br.readLine());
            board = new int[n][n];

            for(int i = 0; i < n; i++){
                StringTokenizer st = new StringTokenizer(br.readLine());
                for (int j = 0; j < n; j++) {
                    board[i][j] = Integer.parseInt(st.nextToken());
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    List<Integer> arr = new ArrayList<>();
                    arr.add(board[i][j]);

                    curY = i;
                    curX = j;

                    moveDown(i, j, 0, 0, 0, arr);
                }
            }
            sb.append("#").append(t).append(" ").append((res != 0)? res : -1).append("\n");
        }



        System.out.println(sb);
    }

}
