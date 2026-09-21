import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.Arrays;
import java.util.StringTokenizer;

public class Solution {
    // 숫자의 최대 크기는 99,999이므로 메모이제이션 배열 크기 설정
    static int[] dp = new int[100000];

    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(brInputStream()));
        
        // 메모이션 배열 -1로 초기화
        Arrays.fill(dp, -1);

        int T = Integer.parseInt(br.readLine().trim());
        StringBuilder sb = new StringBuilder();

        for (int tc = 1; tc <= T; tc++) {
            int N = Integer.parseInt(br.readLine().trim());
            int result = solve(N);
            sb.append("#").append(tc).append(" ").append(result).append("\n");
        }

        System.out.print(sb);
    }

    // InputStream 보조 메서드 (제출 시 System.in으로 변경)
    private static java.io.InputStream brInputStream() {
        return System.in;
    }

    // N에서 시작해서 얻을 수 있는 최대 턴 수를 반환하는 함수
    static int solve(int n) {
        // 한 자리 숫자는 더 이상 쪼갤 수 없으므로 0턴 종료
        if (n < 10) return 0;

        // 이미 계산한 적이 있는 숫자는 저장된 값 반환
        if (dp[n] != -1) return dp[n];

        String str = String.valueOf(n);
        int len = str.length();
        int maxTurns = 0;

        // 자를 수 있는 위치의 개수는 (len - 1)개
        // 비트마스크를 활용해 (1 << (len - 1)) - 1 가지의 분할 경우의 수를 탐색
        int combinations = 1 << (len - 1);

        for (int mask = 1; mask < combinations; mask++) {
            int product = 1;
            int currentChunk = str.charAt(0) - '0';

            for (int i = 0; i < len - 1; i++) {
                // i번째 자리에 구분선을 둘 것인지 확인 (비트가 켜져 있는지 확인)
                if ((mask & (1 << i)) != 0) {
                    product *= currentChunk;
                    currentChunk = str.charAt(i + 1) - '0';
                } else {
                    currentChunk = currentChunk * 10 + (str.charAt(i + 1) - '0');
                }
            }
            // 마지막 조각 곱해주기
            product *= currentChunk;

            // 다음 단계 턴 계산 (재귀 호출)
            maxTurns = Math.max(maxTurns, 1 + solve(product));
        }

        // 결과 메모이제이션 후 반환
        return dp[n] = maxTurns;
    }
}