import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Solution {

    static BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
    static StringTokenizer st;

    static String next() throws Exception {
        while (st == null || !st.hasMoreTokens()) {
            String line = br.readLine();

            if (line == null) return null;

            st = new StringTokenizer(line);
        }

        return st.nextToken();
    }

    public static void main(String[] args) throws Exception {

        int tc = Integer.parseInt(next());

        StringBuilder sb = new StringBuilder();

        for (int t = 1; t <= tc; t++) {

            int n = Integer.parseInt(next());
            int[] arr = new int[n];

            for (int i = 0; i < n; i++) {
                arr[i] = Integer.parseInt(next());
            }

            long cnt = 0;
            int i = 0;

            while (i < n - 1) {

                // 감소 중이라면 다음 위치로
                if (arr[i] > arr[i + 1]) {
                    i++;
                    continue;
                }

                // 증가 구간 시작
                int left = i;

                while (i < n - 1 && arr[i] < arr[i + 1]) {
                    i++;
                }

                // 정점
                int peak = i;

                // 감소 구간 탐색
                while (i < n - 1 && arr[i] > arr[i + 1]) {
                    i++;
                }

                // 저점
                int right = i;

                int up = peak - left;
                int down = right - peak;

                if (up > 0 && down > 0) {
                    cnt += (long) up * down;
                }
            }

            // 테스트케이스당 한 번
            sb.append("#")
              .append(t)
              .append(" ")
              .append(cnt)
              .append("\n");
        }

        // 모든 테스트케이스 처리 후 한 번
        System.out.print(sb);
    }
}