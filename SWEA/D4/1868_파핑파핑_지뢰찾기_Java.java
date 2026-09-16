import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.ArrayDeque;

public class Solution {
    static int[] dx = {-1, 0, 1, 1, 1, 0, -1, -1};
    static int[] dy = {-1, -1, -1, 0, 1, 1, 1, 0};

    static char[][] board;
    static int n;

    // 주변 8방향의 지뢰 개수
    static int findBoom(int y, int x) {
        int boom = 0;

        for (int d = 0; d < 8; d++) {
            int ny = y + dy[d];
            int nx = x + dx[d];

            if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;

            if (board[ny][nx] == '*') {
                boom++;
            }
        }

        return boom;
    }

    // 주변 지뢰가 0개인 칸을 클릭했을 때 연쇄적으로 오픈
    static void clickZero(int y, int x) {
        ArrayDeque<int[]> q = new ArrayDeque<>();

        q.offer(new int[]{y, x});
        board[y][x] = '0';

        while (!q.isEmpty()) {
            int[] cur = q.poll();

            int cy = cur[0];
            int cx = cur[1];

            for (int d = 0; d < 8; d++) {
                int ny = cy + dy[d];
                int nx = cx + dx[d];

                if (ny < 0 || nx < 0 || ny >= n || nx >= n) continue;

                // 아직 열리지 않은 빈 칸만 확인
                if (board[ny][nx] == '.') {
                    int boom = findBoom(ny, nx);

                    // 숫자를 기록하면서 방문 처리
                    board[ny][nx] = (char) (boom + '0');

                    // 주변 지뢰가 0개면 추가 확장
                    if (boom == 0) {
                        q.offer(new int[]{ny, nx});
                    }
                }
            }
        }
    }

    public static void main(String[] args) throws Exception {

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {

            n = Integer.parseInt(br.readLine());

            board = new char[n][n];

            for (int i = 0; i < n; i++) {
                board[i] = br.readLine().toCharArray();
            }

            int res = 0;

            // 1단계
            // 주변 지뢰가 0개인 칸부터 클릭
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {

                    if (board[i][j] == '.' && findBoom(i, j) == 0) {
                        res++;
                        clickZero(i, j);
                    }
                }
            }

            // 2단계
            // 연쇄적으로 열리지 않은 나머지 칸은 각각 한 번씩 클릭
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {

                    if (board[i][j] == '.') {
                        res++;
                    }
                }
            }

            sb.append('#')
                    .append(t)
                    .append(' ')
                    .append(res)
                    .append('\n');
        }

        System.out.print(sb);
    }
}
