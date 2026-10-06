package ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.StringTokenizer;

public class swea1251_하나로_프림 {

    static class Node{
        int from, to;
        long cost;

        Node(int from, int to, long cost){
            this.from = from;
            this.to = to;
            this.cost = cost;
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();
        int tc = 0;
        tc = Integer.parseInt(br.readLine());
        for(int t = 1; t <= tc; t++){
            long sum = 0;
            int n = Integer.parseInt(br.readLine());
            int cnt = 0;
            double E = 0;
            int[] x = new int[n];
            int[] y = new int[n];
            boolean[] vis = new boolean[n + 1];

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
            ArrayList<Node>[] graph = new ArrayList[n + 1];

            for (int i = 0; i < n; i++) {
                graph[i] = new ArrayList<>();
            }

            for(int i = 0; i < n; i++){
                for(int j = i + 1; j < n; j++){
                    long dx = x[i] - x[j];
                    long dy = y[i] - y[j];

                    long dist = dx * dx + dy * dy;

                    graph[i].add(new Node(i, j, dist));
                    graph[j].add(new Node(j, i, dist));
                }
            }

            PriorityQueue<Node> pq = new PriorityQueue<>((a, b) -> Long.compare(a.cost, b.cost));
            pq.add(new Node(0, 0, 0));
            while(!pq.isEmpty()){
                Node cur = pq.poll();
                if(vis[cur.to]) continue;
                vis[cur.to] = true;
                sum += cur.cost;
                cnt++;
                if(cnt == n) break;
                for(Node next : graph[cur.to]){
                    if(!vis[next.to]){
                        pq.offer(next);
                    }
                }
            }
            long answer = Math.round(sum * E);
            sb.append("#").append(t).append(" ").append(answer).append("\n");
        }


        System.out.println(sb);
    }
}
