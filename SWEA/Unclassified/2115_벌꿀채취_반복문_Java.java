package ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class swea2115_벌꿀채취_반복문 {
    public static int dfs(int start, int sum, int value, int c, List<Integer> list) {
        if (sum > c) {
            return 0;
        }

        int mx = value;

        for (int i = start; i < list.size(); i++) {
            int x = list.get(i);
            sum += x;
            value += x * x;

            mx = Math.max(mx, dfs(i + 1, sum, value, c, list));

            sum -= x;
            value -= x * x;
        }

        return mx;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();
        int tc = Integer.parseInt(br.readLine());
        for (int t = 1; t <= tc; t++) {
            StringTokenizer st = new StringTokenizer(br.readLine());
            int n = Integer.parseInt(st.nextToken());
            int m = Integer.parseInt(st.nextToken());
            int c = Integer.parseInt(st.nextToken());
            int res = 0;

            int[][] board = new int[n][n];

            for (int i = 0; i < n; i++) {
                st = new StringTokenizer(br.readLine());
                for (int j = 0; j < n; j++) {
                    board[i][j] = Integer.parseInt(st.nextToken());
                }
            }

            for (int i = 0; i < n; i++) {
                for (int j = 0; j <= n - m; j++) {
                    ArrayList<Integer> list1 = new ArrayList<>();
                    for (int k = j; k < j + m; k++) {
                        list1.add(board[i][k]);
                    }
                    int a = dfs(0, 0, 0, c, list1);

                    for (int y = i; y < n; y++) {
                        int start = 0;
                        if (y == i) {
                            start = j + m;
                        }
                        for (int x = start; x <= n - m; x++) {
                            ArrayList<Integer> list2 = new ArrayList<>();
                            for (int k = x; k < x + m; k++) {
                                list2.add(board[y][k]);
                            }
                            int b = dfs(0, 0, 0, c, list2);
                            res = Math.max(res, a + b);
                        }
                    }
                }
            }

            sb.append("#").append(t).append(" ").append(res).append("\n");
        }
        System.out.println(sb);
    }
}
