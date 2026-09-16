import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Solution {

    static int[][] arr;
    static int res;

    static void dfs(int depth, int y, int x, int n, int sum){
        if(depth == n / 2){
            res += arr[n / 2][n / 2];
            return;
        }
        int size = (n / 2) + 1 - depth;

        for (int i = 0; i < size; i++) {
//            System.out.println(y + " " + x);
            sum += arr[y][x];
//            System.out.println(sum);
            x++;
            y++;
        }
        x -= 2;

//        y--;
//        System.out.println(sum);

        for (int i = 0; i < size - 1; i++) {
//            System.out.println(y + " " + x);
            sum += arr[y][x];
//            System.out.println(sum);
            x--;
            y++;
        }
//        System.out.println(y + " " + x);
        y -= 2;
//        x--;
//        System.out.println(y + " " + x);
//        System.out.println(sum);
//
        for (int i = 0; i < size - 1; i++) {
//            System.out.println(arr[y][x]);
//            System.out.println(y + " " + x);
//            System.out.println(sum);
            sum += arr[y][x];
            x--;
            y--;
        }
//        System.out.println(y + " " + x);
        x += 2;
//        System.out.println(sum);
//        System.out.println(y + " " + x);

        for (int i = 0; i < size - 2; i++) {
//            System.out.println(arr[y][x]);
//            System.out.println(y + " " + x);
//            System.out.println(sum);
            sum += arr[y][x];
            x++;
            y--;
        }
//        System.out.println(y + " " + x);
        res += sum;
        dfs(depth + 1, y + 1, x, n, 0);
    }

    public static void main(String[] args) throws IOException {

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();
        int tc = Integer.parseInt(br.readLine());
        for (int t = 1; t <= tc; t++) {
            res = 0;
            int n = Integer.parseInt(br.readLine());
            arr = new int[n][n];

            for (int i = 0; i < n; i++) {
                String row = br.readLine();
                for (int j = 0; j < n; j++) {
                    arr[i][j] = row.charAt(j) - '0';
                }
            }

            dfs(0, 0, n / 2, n, 0);

            sb.append("#").append(t).append(" ").append(res).append("\n");
        }
        System.out.println(sb);
    }
}
