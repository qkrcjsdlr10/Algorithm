package ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.StringTokenizer;

public class swea5656_벽돌_깨기 {

    static int n = 0, w = 0, h = 0;
    static int[] dx = {0, 1, 0, -1};
    static int[] dy = {-1, 0, 1, 0};
    static int[][] board;
    static int res;

    public static class Block{
        int y, x, area;

        Block(int y, int x, int area){
            this.y = y;
            this.x = x;
            this.area = area;
        }
    }

    public static void dfs(int depth, int n, int broken, int[][] board){
        res = Math.max(res, broken);

        if(depth == n){

//            for (int[] ints : board) {
//                for (int anInt : ints) {
//                    System.out.print(anInt);
//                }
//                System.out.println();
//            }
//            System.out.println(res);
            return;
        }

        for (int c = 0; c < w; c++) {
            int[][] temp = new int[h][w];
            for (int j = 0; j < h; j++) {
                temp[j] = board[j].clone();
            }

            int r = searchRow(c, temp);
            if(r == -1) continue;
            int cnt = 0;

            ArrayDeque<Block> q = new ArrayDeque<>();
            q.addLast(new Block(r, c, temp[r][c]));

            temp[r][c] = 0;
            while(!q.isEmpty()){
                Block cur = q.pollFirst();
                int y = cur.y;
                int x = cur.x;
                int area = cur.area;
                cnt++;

                for (int d = 0; d < 4; d++) {
                    for (int i = 1; i < area; i++) {
                        int nx = x + (dx[d] * i);
                        int ny = y + (dy[d] * i);

                        if(ny >= h || ny < 0 || nx >= w || nx < 0) continue;
                        if(temp[ny][nx] == 0) continue;

                        q.addLast(new Block(ny, nx, temp[ny][nx]));
                        temp[ny][nx] = 0;
                    }
                }

            }

            for (int i = h - 1; i >= 0; i--) {
                for(int j = w - 1; j >= 0; j--){
                    if(temp[i][j] == 0){
                        for (int k = i - 1; k >= 0 ; k--) {
                            if(temp[k][j] != 0){
                                temp[i][j] = temp[k][j];
                                temp[k][j] = 0;
                                break;
                            }
                        }
                    }
                }
            }

            dfs(depth + 1, n, broken + cnt, temp);
        }


    }

    public static int searchRow(int col, int[][] board){
        for (int i = 0; i < h; i++) {
            if(board[i][col] != 0){
                return i;
            }
        }
        return -1;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());
        for(int t = 1; t <= tc; t++){
            StringTokenizer st = new StringTokenizer(br.readLine());

            res = 0;

            n = Integer.parseInt(st.nextToken());
            w = Integer.parseInt(st.nextToken());
            h = Integer.parseInt(st.nextToken());

            board = new int[h][w];
            int blockCnt = 0;

            for (int i = 0; i < h; i++) {
                st = new StringTokenizer(br.readLine());
                for (int j = 0; j < w; j++) {
                    board[i][j] = Integer.parseInt(st.nextToken());
                    if(board[i][j] != 0){
                        blockCnt++;
                    }
                }
            }

            dfs(0, n, 0, board);
            sb.append("#").append(t).append(" ").append(blockCnt - res).append("\n");
        }



        System.out.println(sb);
    }

}
