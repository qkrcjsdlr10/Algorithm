package other;

import java.io.*;
import java.util.*;

public class swea1247_최적_경로_dfs {

    static int N;
    static int answer;
    static ArrayList<Node> nodes;

    static class Node {
        int x, y;

        Node(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }

    static int getDist(int a, int b) {
        Node A = nodes.get(a);
        Node B = nodes.get(b);

        int dx = Math.abs(A.x - B.x);
        int dy = Math.abs(A.y - B.y);

        return dx + dy;
    }

    static void dfs(int depth, int flag, int cur, int sum) {
        if (sum >= answer) return;

        if (depth == N) {
            sum += getDist(cur, N + 1);
            answer = Math.min(answer, sum);
            return;
        }

        for (int i = 1; i <= N; i++) {
            if ((flag & (1 << i)) != 0) continue;

            int nextFlag = flag | (1 << i);
            int nextSum = sum + getDist(cur, i);

            dfs(depth + 1, nextFlag, i, nextSum);
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {

            N = Integer.parseInt(br.readLine());
            nodes = new ArrayList<>();
            answer = Integer.MAX_VALUE;

            StringTokenizer st = new StringTokenizer(br.readLine());

            int x = Integer.parseInt(st.nextToken());
            int y = Integer.parseInt(st.nextToken());
            nodes.add(new Node(x, y));

            int homeX = Integer.parseInt(st.nextToken());
            int homeY = Integer.parseInt(st.nextToken());

            for (int i = 0; i < N; i++) {
                x = Integer.parseInt(st.nextToken());
                y = Integer.parseInt(st.nextToken());

                nodes.add(new Node(x, y));
            }

            nodes.add(new Node(homeX, homeY));

            dfs(0, 0, 0, 0);

            sb.append("#").append(t).append(" ").append(answer).append("\n");
        }

        System.out.print(sb);
    }
}