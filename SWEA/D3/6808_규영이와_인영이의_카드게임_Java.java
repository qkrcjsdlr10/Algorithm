import java.io.*;
import java.util.*;

public class Solution {

    static int[] gyu = new int[9];
    static int[] in = new int[9];
    static boolean[] vis = new boolean[9];

    static int[] fact = new int[10];

    static int win;
    static int lose;

    public static void dfs(int depth, int gyuScore, int inScore) {
        if (gyuScore > 85) {
            win += fact[9 - depth];
            return;
        }
        if (inScore > 85) {
            lose += fact[9 - depth];
            return;
        }

        if (depth == 9) {
            if (gyuScore > inScore) {
                win++;
            } else {
                lose++;
            }
            return;
        }

        for (int i = 0; i < 9; i++) {
            if (vis[i]) continue;

            vis[i] = true;

            int sum = gyu[depth] + in[i];

            if (gyu[depth] > in[i]) {
                dfs(depth + 1, gyuScore + sum, inScore);
            } else {
                dfs(depth + 1, gyuScore, inScore + sum);
            }

            vis[i] = false;
        }
    }

    public static void main(String[] args) throws IOException {

        BufferedReader br = new BufferedReader(
                new InputStreamReader(System.in)
        );

        fact[0] = 1;

        for (int i = 1; i <= 9; i++) {
            fact[i] = fact[i - 1] * i;
        }

        int T = Integer.parseInt(br.readLine());

        for (int tc = 1; tc <= T; tc++) {

            StringTokenizer st = new StringTokenizer(br.readLine());

            boolean[] used = new boolean[19];

            for (int i = 0; i < 9; i++) {
                gyu[i] = Integer.parseInt(st.nextToken());
                used[gyu[i]] = true;
            }

            int idx = 0;

            for (int i = 1; i <= 18; i++) {
                if (!used[i]) {
                    in[idx++] = i;
                }
            }

            vis = new boolean[9];
            win = 0;
            lose = 0;

            dfs(0, 0, 0);

            System.out.println("#" + tc + " " + win + " " + lose);
        }
    }
}