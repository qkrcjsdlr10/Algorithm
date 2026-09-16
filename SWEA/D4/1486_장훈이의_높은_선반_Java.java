import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Solution {

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuffer sb = new StringBuffer();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {

            StringTokenizer st = new StringTokenizer(br.readLine());

            int mx = 0;
            int n = Integer.parseInt(st.nextToken());
            int target = Integer.parseInt(st.nextToken());
            int[] arr = new int[n];

            st = new StringTokenizer(br.readLine());

            for (int i = 0; i < n; i++) {
                arr[i] = Integer.parseInt(st.nextToken());
                mx += arr[i];
            }

            int[][] dp = new int[n + 1][mx + 1];
            dp[0][0] = 1;

            for (int i = 1; i <= n; i++) {
                int h = arr[i - 1];
                for (int j = 0; j <= mx; j++) {
                    dp[i][j] = dp[i - 1][j];

                    if (j >= h) {
                        dp[i][j] |= dp[i - 1][j - h];
                    }
                }
            }
            int curMx = 0;
            for(int i = target; i <= mx; i++){
                if(dp[n][i] == 1){
                    curMx = i;
                    break;
                }
            }
            sb.append("#").append(t).append(" ").append(curMx - target).append("\n");
        }
        System.out.println(sb);
    }
}
