import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.StringTokenizer;

public class Solution {

    static List<List<Integer>> graph;
    static int res = 0;

    public static void dfs(int n, int start, int isSelect){
        res++;

        for(int i = start; i <= n; i++){
            boolean isValid = true;

            for(int v : graph.get(i)){
                if((isSelect & (1 << v)) != 0){
                    isValid = false;
                    break;
                }
            }
            if(!isValid) continue;

            dfs(n, i + 1, isSelect | (1 << i));
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int tc = Integer.parseInt(br.readLine());
        for (int t = 1; t <= tc; t++) {
            int n = 0, m = 0;
            res = 0;
            StringTokenizer st = new StringTokenizer(br.readLine());
            graph = new ArrayList<>();

            n = Integer.parseInt(st.nextToken());
            m = Integer.parseInt(st.nextToken());

            for(int i = 0; i <= n; i++){
                graph.add(new ArrayList<>());
            }

            for (int i = 0; i < m; i++) {
                st = new StringTokenizer(br.readLine());
                int u = Integer.parseInt(st.nextToken());
                int v = Integer.parseInt(st.nextToken());

                graph.get(u).add(v);
                graph.get(v).add(u);
            }

            dfs(n, 1, 0);

            System.out.println("#"+ t + " " + res);
        }

    }
}
