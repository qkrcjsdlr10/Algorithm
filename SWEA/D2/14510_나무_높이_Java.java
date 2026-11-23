import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Solution {

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            int n = Integer.parseInt(br.readLine());
            int[] arr = new int[n];

            int mx = 0;
            StringTokenizer st = new StringTokenizer(br.readLine());

            for (int i = 0; i < n; i++) {
                arr[i] = Integer.parseInt(st.nextToken());
                mx = Math.max(mx, arr[i]);
            }

            int one = 0;
            int two = 0;

            for (int x : arr) {
                int diff = mx - x;

                one += diff % 2;
                two += diff / 2;
            }
            while (two > one + 1) {
                two--;
                one += 2;
            }

            int res;

            if (one > two) {
                res = one * 2 - 1;
            } else {
                res = two * 2;
            }

            System.out.println("#" + t + " " + res);
        }
    }
}