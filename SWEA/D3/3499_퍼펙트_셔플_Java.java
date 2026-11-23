import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayDeque;
import java.util.StringTokenizer;

public class Solution {

    public static void main(String[] args) throws IOException {

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            int n = Integer.parseInt(br.readLine());
            StringTokenizer st = new StringTokenizer(br.readLine());

            ArrayDeque<String> q1 = new ArrayDeque<>();
            ArrayDeque<String> q2 = new ArrayDeque<>();

            int half = (n + 1) / 2;

            for (int i = 0; i < n; i++) {
                String card = st.nextToken();

                if (i < half) {
                    q1.addLast(card);
                } else {
                    q2.addLast(card);
                }
            }

            StringBuilder sb = new StringBuilder();
            sb.append("#").append(t).append(" ");

            while (!q1.isEmpty() || !q2.isEmpty()) {
                if (!q1.isEmpty()) {
                    sb.append(q1.pollFirst()).append(" ");
                }
                if (!q2.isEmpty()) {
                    sb.append(q2.pollFirst()).append(" ");
                }
            }

            System.out.println(sb);
        }
    }
}