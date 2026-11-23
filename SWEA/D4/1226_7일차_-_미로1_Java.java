import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;

public class Solution {

    static class Pair{
        int first;
        int second;

        Pair(int first, int second){
            this.first = first;
            this.second = second;
        }
    }

    public static void main(String[] args) throws IOException {
        int[] dx = {0, 1, 0, -1};
        int[] dy = {-1, 0, 1, 0};
        int tc = 10;
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        for (int t = 1; t <= tc; t++) {
            int tt = Integer.parseInt(br.readLine());
            char[][] board = new char[16][16];
            int sx = 0, sy = 0, ex = 0, ey = 0;
            ArrayDeque<Pair> q = new ArrayDeque<>();

            for(int i = 0; i < 16; i++){
                board[i] = br.readLine().toCharArray();
                for (int j = 0; j < 16; j++) {
                    if (board[i][j] == '2') {
                        sx = j;
                        sy = i;
                    }else if(board[i][j] == '3'){
                        ex = j;
                        ey = i;
                    }
                }
            }

            q.addLast(new Pair(sy, sx));

            while(!q.isEmpty()){
                Pair cur = q.pollFirst();
                int x = cur.second;
                int y = cur.first;

                for(int d = 0; d < 4; d++){
                    int nx = x + dx[d];
                    int ny = y + dy[d];

                    if(ny < 0 || ny >= 16 || nx < 0 || nx >= 16) continue;
                    if(board[ny][nx] == '1') continue;

                    board[ny][nx] = '1';
                    q.addLast(new Pair(ny, nx));
                }
            }

            System.out.println("#" + tt + " " +((board[ey][ex] == '1') ? 1 : 0));
        }
    }
}
