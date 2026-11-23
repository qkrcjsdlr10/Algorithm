import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class Solution {

    static int[] dy = {0, -1, 0, 1, 0};
    static int[] dx = {0, 0, 1, 0, -1};

    static List<Integer>[][] board;
    static int[][] spots;

    static class Player {
        int y;
        int x;

        Player(int y, int x) {
            this.y = y;
            this.x = x;
        }
    }

    static public void checkArea(int y, int x, int c, int idx) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (Math.abs(x - j) + Math.abs(y - i) <= c) {
                    board[i][j].add(idx);
                }
            }
        }
    }

    static public int charge(Player a, Player b) {
        List<Integer> aList = board[a.y][a.x];
        List<Integer> bList = board[b.y][b.x];

        int max = 0;

        if (aList.isEmpty()) {
            for (int bv : bList) {
                max = Math.max(max, spots[bv][3]);
            }
        } else if (bList.isEmpty()) {
            for (int av : aList) {
                max = Math.max(max, spots[av][3]);
            }
        } else {
            for (int av : aList) {
                for (int bv : bList) {
                    int sum;

                    if (av == bv) {
                        sum = spots[av][3];
                    } else {
                        sum = spots[av][3] + spots[bv][3];
                    }

                    max = Math.max(max, sum);
                }
            }
        }

        return max;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            StringTokenizer st = new StringTokenizer(br.readLine());

            int n = Integer.parseInt(st.nextToken());
            int spot = Integer.parseInt(st.nextToken());

            int[] player1 = new int[n];
            int[] player2 = new int[n];

            Player a = new Player(0, 0);
            Player b = new Player(9, 9);

            board = new ArrayList[10][10];
            spots = new int[spot][4];

            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    board[i][j] = new ArrayList<>();
                }
            }

            st = new StringTokenizer(br.readLine());
            for (int i = 0; i < n; i++) {
                player1[i] = Integer.parseInt(st.nextToken());
            }

            st = new StringTokenizer(br.readLine());
            for (int i = 0; i < n; i++) {
                player2[i] = Integer.parseInt(st.nextToken());
            }

            for (int i = 0; i < spot; i++) {
                st = new StringTokenizer(br.readLine());

                for (int j = 0; j < 4; j++) {
                    spots[i][j] = Integer.parseInt(st.nextToken());
                }
            }

            for (int i = 0; i < spot; i++) {
                checkArea(spots[i][1] - 1, spots[i][0] - 1, spots[i][2], i);
            }

            int res = 0;

            // 시작 위치에서 충전
            res += charge(a, b);

            // 이동 후 충전
            for (int i = 0; i < n; i++) {
                a.y += dy[player1[i]];
                a.x += dx[player1[i]];

                b.y += dy[player2[i]];
                b.x += dx[player2[i]];

                res += charge(a, b);
            }

            sb.append("#").append(t).append(" ").append(res).append("\n");
        }

        System.out.print(sb);
    }
}
