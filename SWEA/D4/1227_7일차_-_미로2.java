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

    static int[] dx = {0, 1, 0, -1};
    static int[] dy = {-1, 0, 1, 0};

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();
        for(int tc = 0; tc < 10; tc++){
            int t = Integer.parseInt(br.readLine());
            char[][] board = new char[100][100];
            int sx = 0, sy = 0, ex = 0, ey = 0;

            for(int i = 0; i < 100; i++){
                String s = br.readLine();
                if(s.indexOf("2") != -1){
                    sx = s.indexOf("2");
                    sy = i;
                }else if(s.indexOf("3") != -1){
                    ex = s.indexOf("3");
                    ey = i;
                }
                board[i] = s.toCharArray();
            }

            ArrayDeque<Pair> q = new ArrayDeque<>();
            q.add(new Pair(sy, sx));
            board[sy][sx] = '1';
            int cnt = 0;

            while(!q.isEmpty()){
//            if(cnt == 20) break;
//            cnt++;
                Pair cur = q.pollFirst();
                int x = cur.second;
                int y = cur.first;
//            System.out.println(y + " " + x);

                for(int d = 0; d < 4; d++){
                    int nx = x + dx[d];
                    int ny = y + dy[d];

                    if(nx < 0 || nx >= 100 || ny < 0 || ny >= 100) continue;
                    if(board[ny][nx] == '1') continue;

                    board[ny][nx] = '1';
                    q.addLast(new Pair(ny, nx));
                }
            }

            sb.append("#").append(t).append(" ").append((board[ey][ex] == '1') ? 1 : 0).append("\n");
        }

        System.out.println(sb);
    }

}
