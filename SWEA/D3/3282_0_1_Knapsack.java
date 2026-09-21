import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Solution {

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int tc = Integer.parseInt(br.readLine());

        for(int t = 1; t <= tc; t++){
            StringTokenizer st = new StringTokenizer(br.readLine());

            int n = Integer.parseInt(st.nextToken());
            int k = Integer.parseInt(st.nextToken());
            int mx = 0;
            int res = 0;

            int[][] arr = new int[n][2];

            for (int i = 0; i < n; i++) {
                st = new StringTokenizer(br.readLine());
                arr[i][0] = Integer.parseInt(st.nextToken());
                arr[i][1] = Integer.parseInt(st.nextToken());

                mx += arr[i][0];
            }

            int[][] dp = new int[n + 1][k + 1];
            for(int i = 1; i <= n; i++){
                for(int w = 0; w <= k; w++){
                    int weight = arr[i - 1][0];
                    int value = arr[i - 1][1];

                    if(w < weight){
                        dp[i][w] = dp[i - 1][w];
                    }else{
                        dp[i][w] = Math.max(dp[i - 1][w], dp[i - 1][w - weight] + value);
                    }
                }
            }

            System.out.println("#" + t + " " + dp[n][k]);
        }

    }
}
