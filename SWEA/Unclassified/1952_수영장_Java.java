import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class Solution {

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();
        int tc = Integer.parseInt(br.readLine());
        for (int t = 1; t <= tc; t++) {
            int[] cost = new int[4];
            int[] day = new int[12];
            int[] dp = new int[13];

            StringTokenizer st = new StringTokenizer(br.readLine());

            for (int i = 0; i < 4; i++) {
                cost[i] = Integer.parseInt(st.nextToken());
            }
            st = new StringTokenizer(br.readLine());
            for(int i = 0; i < 12; i++){
                day[i] = Integer.parseInt(st.nextToken());
            }

            dp[1] = Math.min(day[0] * cost[0], cost[1]);
            dp[2] = Math.min(dp[1] + day[1] * cost[0], dp[1] + cost[1]);


            for (int i = 3; i < 13; i++) {
                dp[i] = Math.min(dp[i - 1] +  Math.min(day[i - 1] * cost[0], cost[1]), dp[i - 3] + cost[2]);
            }

            dp[12] = Math.min(dp[12], cost[3]);
            sb.append("#").append(t).append(" ").append(dp[12]).append("\n");
        }
        System.out.println(sb);
    }
}
