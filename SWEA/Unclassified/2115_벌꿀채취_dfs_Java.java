package ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class swea2115_벌꿀채취_dfs {
    static int n, m, c;
    static int[][] board;
    static int res;

    // 한 구간 내부에서 C 이하 부분집합 최대 수익
    public static int honeyDfs(int start, int sum, int value, List<Integer> list) {
        if (sum > c) return 0;

        int mx = value;

        for (int i = start; i < list.size(); i++) {
            int x = list.get(i);

            sum += x;
            value += x * x;

            mx = Math.max(mx, honeyDfs(i + 1, sum, value, list));

            sum -= x;
            value -= x * x;
        }

        return mx;
    }

    // M칸 구간 2개 선택
    public static void dfs(int y, int x, int cnt, int value) {

        if (cnt == 2) {
            res = Math.max(res, value);
            return;
        }

        for (int i = y; i < n; i++) {

            int start = 0;

            if (i == y) {
                start = x;
            }

            for (int j = start; j <= n - m; j++) {

                List<Integer> list = new ArrayList<>();

                for (int k = j; k < j + m; k++) {
                    list.add(board[i][k]);
                }

                int mx = honeyDfs(0, 0, 0, list);

                // 같은 행에서는 현재 구간 뒤부터 탐색
                dfs(i, j + m, cnt + 1, value + mx);
            }
        }
    }

    public static void main(String[] args) throws IOException {

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int T = Integer.parseInt(br.readLine());

        for (int tc = 1; tc <= T; tc++) {

            StringTokenizer st = new StringTokenizer(br.readLine());

            n = Integer.parseInt(st.nextToken());
            m = Integer.parseInt(st.nextToken());
            c = Integer.parseInt(st.nextToken());

            board = new int[n][n];

            for (int i = 0; i < n; i++) {
                st = new StringTokenizer(br.readLine());

                for (int j = 0; j < n; j++) {
                    board[i][j] = Integer.parseInt(st.nextToken());
                }
            }

            res = 0;

            dfs(0, 0, 0, 0);

            sb.append("#").append(tc).append(" ").append(res).append("\n");
        }

        System.out.print(sb);
    }
}
