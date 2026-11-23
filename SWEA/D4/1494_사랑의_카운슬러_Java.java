package swea;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class Swea1494 {

    static ArrayList<Pair> arr;
    static long mn = Long.MAX_VALUE;

    public static class Pair{
        int first, second;

        Pair(int first, int second){
            this.first = first;
            this.second = second;
        }
    }

    public static void dfs(int depth, int start,  int n, ArrayList<Integer> temp){
        if(depth == n / 2){
            ArrayList<Integer> temp2 = new ArrayList<>();
            boolean[] vis = new boolean[n];
            for (int i = 0; i < temp.size(); i++) {
                vis[temp.get(i)] = true;
            }

            for (int i = 0; i < n; i++) {
                if(!vis[i]) temp2.add(i);
            }

            long sum = getVectorSum(temp, temp2);
            mn = Math.min(mn, sum);

            return;
        }

        for (int i = start; i < n; i++) {
            temp.add(i);
            dfs(depth + 1, i + 1, n, temp);
            temp.remove(temp.size() - 1);
        }
    }

    public static long getVectorSum(ArrayList<Integer> temp1, ArrayList<Integer> temp2){
        int sumX1 = 0;
        int sumX2 = 0;
        int sumY1 = 0;
        int sumY2 = 0;

        for(int x : temp1){
            sumX1 += arr.get(x).first;
            sumY1 += arr.get(x).second;
        }

        for(int x : temp2){
            sumX2 += arr.get(x).first;
            sumY2 += arr.get(x).second;
        }

        long dx = (long) sumX1 - sumX2;
        long dy = (long) sumY1 - sumY2;

        return dx * dx + dy * dy;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();


        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            StringTokenizer st = new StringTokenizer(br.readLine());
            int n = Integer.parseInt(st.nextToken());

            arr = new ArrayList<>();
            mn = Long.MAX_VALUE;

            for (int i = 0; i < n; i++) {
                st = new StringTokenizer(br.readLine());
                int first = Integer.parseInt(st.nextToken());
                int second = Integer.parseInt(st.nextToken());
                arr.add(new Pair(first, second));
            }

            dfs(0, 0, n, new ArrayList<>());

            sb.append("#").append(t).append(" ").append(mn).append("\n");
        }
        System.out.println(sb);


    }

}
