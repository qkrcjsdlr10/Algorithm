package hw;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class swea3124_최소_스패닝_트리 {

    static int[] p;

    static int find(int a){
        if(p[a] == a) return a;
        return p[a] = find(p[a]);
    }
    
    static boolean union(int a, int b){
        a = find(a);
        b = find(b);
        
        if(a == b) return false;
        
        p[b] = a;
        return true;
    }
    
    static class Edge{
        int from, to, cost;
        
        Edge(int from, int to, int cost){
            this.from = from;
            this.to = to;
            this.cost = cost;
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            StringTokenizer st = new StringTokenizer(br.readLine());


            int V = Integer.parseInt(st.nextToken());
            int E = Integer.parseInt(st.nextToken());
            ArrayList<Edge> edges = new ArrayList<>();
            p = new int[V + 1];

            for (int i = 0; i <= V; i++) {
                p[i] = i;
            }

            long sum = 0;
            int cnt = 0;

            for (int i = 0; i < E; i++) {
                st = new StringTokenizer(br.readLine());
                int from = Integer.parseInt(st.nextToken());
                int to = Integer.parseInt(st.nextToken());
                int cost = Integer.parseInt(st.nextToken());

                edges.add(new Edge(from, to, cost));
            }

            edges.sort((a, b) -> Integer.compare(a.cost, b.cost));

            for (Edge edge : edges) {
                if(find(edge.from) == find(edge.to)) continue;
                union(edge.from, edge.to);

                sum += edge.cost;
                cnt++;
                if(cnt == V - 1) break;
            }

            sb.append("#").append(t).append(" ").append(sum).append("\n");
        }

        System.out.println(sb);

    }
}
