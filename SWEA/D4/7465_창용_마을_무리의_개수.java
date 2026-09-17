package ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.HashMap;
import java.util.HashSet;
import java.util.StringTokenizer;

public class swea7465_창용_마을_무리의_개수 {

    static int[] p;

    public static int find(int x){
        if(p[x] == x) return x;
        return p[x] = find(p[x]);
    }

    public static void union(int a, int b){
        a = find(a);
        b = find(b);

        if(a == b) return;
        p[b] = a;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());
        for(int t = 1; t <= tc; t++){
            StringTokenizer st = new StringTokenizer(br.readLine());
            HashSet<Integer> vis = new HashSet<>();
            int res = 0;
            int n = Integer.parseInt(st.nextToken());
            int m = Integer.parseInt(st.nextToken());

            p = new int[n + 1];

            for (int i = 1; i <= n; i++) {
                p[i] = i;
            }

            for (int i = 0; i < m; i++) {
                st = new StringTokenizer(br.readLine());
                int a = Integer.parseInt(st.nextToken());
                int b = Integer.parseInt(st.nextToken());

                union(a, b);
            }
            int cur = 0;

            for (int i = 1; i <= n; i++) {
                if(!vis.contains(p[i])){
                    vis.add(find(i));
                }
            }
            sb.append("#").append(t).append(" ").append(vis.size()).append("\n");
        }
        System.out.println(sb);
    }
}
