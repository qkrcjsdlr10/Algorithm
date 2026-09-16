import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Solution {
    public static void main(String[] args) throws IOException {

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int tc = Integer.parseInt(br.readLine());
        for(int t = 1; t <= tc; t++){
            int n = 0, m = 0;

            StringTokenizer st = new StringTokenizer(br.readLine());
            n = Integer.parseInt(st.nextToken());
            m = Integer.parseInt(st.nextToken());
            int mx = 0;

            int[][] board = new int[n][n];
            int[][] prefix = new int[n + 1][n + 1];

            for(int i = 0; i < n; i++){
                st = new StringTokenizer(br.readLine());
                for(int j = 0; j < n; j++){
                    board[i][j] = Integer.parseInt(st.nextToken());
                }
            }

            for(int i = 1; i <= n; i++){
                for(int j = 1; j <= n; j++){
                    prefix[i][j] = prefix[i][j-1] + prefix[i-1][j] - prefix[i-1][j-1] + board[i - 1][j - 1];
                }
            }

            for(int i = m; i <= n; i++){
                for(int j = m; j <= n; j++){
                    int sum = prefix[i][j] - prefix[i-m][j] - prefix[i][j-m] + prefix[i-m][j-m];
                    mx = Math.max(sum, mx);
                }
            }
            System.out.println("#" + t + " " +mx);
        }
    }
}
