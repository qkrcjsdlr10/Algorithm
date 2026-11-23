package hw;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class swea5643_키_순서 {

    public static Integer dfs(ArrayList<Integer>[] graph, boolean[] vis, int cur, int cnt){
        for (Integer next : graph[cur]) {
            if(vis[next]) continue;
            vis[next] = true;
            cnt = dfs(graph, vis, next, cnt + 1);
        }
        return cnt;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            StringTokenizer st = new StringTokenizer(br.readLine());
            int n = Integer.parseInt(st.nextToken());
            st = new StringTokenizer(br.readLine());
            int m = Integer.parseInt(st.nextToken());

            ArrayList<Integer>[] winGraph = new ArrayList[n + 1];
            ArrayList<Integer>[] loseGraph = new ArrayList[n + 1];

            for (int i = 0; i <= n; i++) {
                winGraph[i] = new ArrayList<>();
                loseGraph[i] = new ArrayList<>();
            }

            for (int i = 0; i < m; i++) {
                st = new StringTokenizer(br.readLine());
                int u = Integer.parseInt(st.nextToken());
                int v = Integer.parseInt(st.nextToken());
                winGraph[u].add(v);
                loseGraph[v].add(u);
            }

            int answer = 0;

            for (int i = 1; i <= n; i++) {
                boolean[] vis = new boolean[n + 1];
                vis[i] = true;
                int winCnt = dfs(winGraph, vis, i, 0);
                vis = new boolean[n + 1];
                vis[i] = true;
                int loseCnt = dfs(loseGraph, vis, i, 0);
                if(winCnt + loseCnt == n - 1) answer++;
            }

            sb.append("#").append(t).append(" ").append(answer).append("\n");
        }



        System.out.println(sb);
    }

}
