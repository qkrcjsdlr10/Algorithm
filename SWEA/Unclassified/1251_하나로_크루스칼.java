package ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class swea1251_하나로_크루스칼 {

    static int[] p;

    static class Edge implements Comparable<Edge>{
        int from, to;
        long cost;

        Edge(int from, int to, long cost){
            this.from = from;
            this.to = to;
            this.cost = cost;
        }

        @Override
        public int compareTo(Edge o) {
            return Long.compare(this.cost, o.cost);
        }
    }

    static int find(int a){
        if(p[a] == a) return a;
        return p[a] = find(p[a]);
    }

    static void union(int a, int b){
        a = find(a);
        b = find(b);

        if(a == b) return;

        p[b] = a;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();
        int tc = 0;
        tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            long sum = 0;
            int n = Integer.parseInt(br.readLine());
            int cnt = 0;
            double E = 0;
            int[] x = new int[n];
            int[] y = new int[n];
            p = new int[n];

            for(int i = 0; i < n; i++){
                p[i] = i;
            }

            ArrayList<Edge> edges = new ArrayList<>();

            for(int i = 0; i < 2; i++){
                StringTokenizer st = new StringTokenizer(br.readLine());
                for(int j = 0; j < n; j++){
                    if(i == 0){
                        x[j] = Integer.parseInt(st.nextToken());
                    }else{
                        y[j] = Integer.parseInt(st.nextToken());
                    }
                }
            }
            E = Double.parseDouble(br.readLine());
            for(int i = 0; i < n; i++){
                for(int j = i + 1; j < n; j++){
                    long dx = x[i] - x[j];
                    long dy = y[i] - y[j];

                    long dist = dx * dx + dy * dy;

                    edges.add(new Edge(i, j, dist));
                }
            }

            edges.sort(null);

            for(Edge e : edges){
                if(find(e.from) == find(e.to)) continue;

                union(e.from, e.to);
                sum += e.cost;
                cnt++;

                if(cnt == n - 1) break;
            }
            long answer = Math.round(sum * E);
            sb.append("#").append(t).append(" ").append(answer).append("\n");
        }

        System.out.println(sb);
    }
}
