import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.StringTokenizer;

public class Solution {

    static int[] dx = {-1, 1, 0};
    static int[] dy = {0, 0, 1};
    static ArrayDeque<Node> q;
    static int[][] board = new int[100][100];
    static int[][] temp;

    static class Node{
        int first;
        int second;

        Node(int first, int second){
            this.first = first;
            this.second = second;
        }
    }

    public static boolean bfs(int[][] temp, int ey, int ex){
//        System.out.println(temp[ey][ex]);
        while(!q.isEmpty()){
            Node cur = q.pollFirst();
            int y = cur.first;
            int x = cur.second;
            boolean check = false;

            for(int d = 0; d < 3; d++){
                if(check) continue;
                int nx = x + dx[d];
                int ny = y + dy[d];
//                System.out.println(ny + " " + nx);
                if(ny >= 100 || nx < 0 || nx >= 100) continue;
                if(temp[ny][nx] == 0) continue;
                if (ny == ey && nx == ex) return true;
                if(d == 0 || d == 1) check = true;

                q.addLast(new Node(ny, nx));
                temp[ny][nx] = 0;
//                System.out.println(ny + " " + nx + " " + temp[ny][nx]);
            }
        }
//        System.out.println(temp[ey][ex]);
        return false;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        for(int t = 0; t < 10; t++){
            int tc = Integer.parseInt(br.readLine());

            int answer = -1;
            q = new ArrayDeque<>();
            int ex = 0, ey = 0;
            for (int i = 0; i < 100; i++) {
                StringTokenizer st = new StringTokenizer(br.readLine());
                for (int j = 0; j < 100; j++) {
                    board[i][j] = st.nextToken().charAt(0) - '0';
                    if(board[i][j] == 2){
                        ex = j;
                        ey = i;
                    }
                }
            }

            for (int i = 0; i < 100; i++) {
                int[][] temp = new int[board.length][];

                if(board[0][i] == 1 && board[1][i] == 1){
                    for (int j = 0; j < board.length; j++) {
                        temp[j] = board[j].clone();
                    }
                    q.addLast(new Node(1, i));
                    temp[1][i] = 0;
                    if(bfs(temp, ey, ex)){
                        answer = i;
                        break;
                    }
                }
            }

            System.out.println("#" + tc + " " + answer);
        }
    }
}
