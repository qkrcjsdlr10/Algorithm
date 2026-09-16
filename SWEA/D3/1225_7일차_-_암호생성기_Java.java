import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.Queue;
import java.util.StringTokenizer;

public class Solution {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        int T = 10;

        for (int tc = 1; tc <= T; tc++) {
            int t = Integer.parseInt(br.readLine());
            StringTokenizer st = new StringTokenizer(br.readLine());

            Queue<Integer> q = new ArrayDeque<>();

            while (st.hasMoreTokens()) {
                q.add(Integer.parseInt(st.nextToken()));
            }

            int dec = 1;

            while (true) {
                int cur = q.poll();

                if (cur - dec <= 0) {
                    q.add(0);
                    break;
                }

                q.add(cur - dec);

                dec++;
                if (dec > 5) dec = 1;
            }

            System.out.print("#" + t + " ");

            while (!q.isEmpty()) {
                System.out.print(q.poll() + " ");
            }
            System.out.println();
        }
    }
}
