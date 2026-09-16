import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.StringTokenizer;

public class Solution {

    static int n = 0;
    static int mxWeight = 0;
    static int res = 0;
    static int[][] arr;
    static int[][] dp;

    public static int dfs(int depth, int w){
        if(depth == n){
            return 0;
        }
        if(w > mxWeight) return 0;

        if(dp[depth][w] != -1){
            return dp[depth][w];
        }

        int notSelect = dfs(depth + 1, w);
        int select = 0;
        
        if (w + arr[depth][1] <= mxWeight) {
            select = arr[depth][0] + dfs(depth + 1, w + arr[depth][1]);
        }

        dp[depth][w] = Math.max(notSelect, select);

        return dp[depth][w];
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            StringTokenizer st = new StringTokenizer(br.readLine());

            n = Integer.parseInt(st.nextToken());
            mxWeight = Integer.parseInt(st.nextToken());
            res = 0;
            arr = new int[n][2];
            dp = new int[n][mxWeight + 1];
            for(int i = 0; i < n; i++){
                st = new StringTokenizer(br.readLine());
                arr[i][0] = Integer.parseInt(st.nextToken());
                arr[i][1] = Integer.parseInt(st.nextToken());
            }

            for (int i = 0; i < n; i++) {
                Arrays.fill(dp[i], -1);
            }

            int res = dfs(0, 0);

            System.out.println("#" + t + " " + res);
        }
    }
}
