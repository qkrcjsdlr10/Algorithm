import java.io.*;
import java.util.*;

public class Solution {

    static int[] dx = {0, 1, 0, -1};
    static int[] dy = {-1, 0, 1, 0};

    static int n;
    static char[][] map;
    static List<Pair> cores;

    static int maxCore;
    static int minLine;

    static class Pair {
        int y;
        int x;

        Pair(int y, int x) {
            this.y = y;
            this.x = x;
        }
    }

    static void dfs(int depth, int coreCnt, int lineCnt) {
        if (depth == cores.size()) {
            if (coreCnt > maxCore) {
                maxCore = coreCnt;
                minLine = lineCnt;
            } else if (coreCnt == maxCore) {
                minLine = Math.min(minLine, lineCnt);
            }
            return;
        }

        Pair cur = cores.get(depth);

        for (int d = 0; d < 4; d++) {
            int x = cur.x;
            int y = cur.y;

            while (true) {
                int nx = x + dx[d];
                int ny = y + dy[d];

                if (map[ny][nx] == '#') {
                    int len = 0;
                    int tx = cur.x + dx[d];
                    int ty = cur.y + dy[d];

                    while (map[ty][tx] != '#') {
                        map[ty][tx] = '1';
                        len++;
                        tx += dx[d];
                        ty += dy[d];
                    }

                    dfs(depth + 1, coreCnt + 1, lineCnt + len);

                    tx -= dx[d];
                    ty -= dy[d];

                    while (ty != cur.y || tx != cur.x) {
                        map[ty][tx] = '0';
                        tx -= dx[d];
                        ty -= dy[d];
                    }

                    break;
                }

                if (map[ny][nx] == '1') break;

                x = nx;
                y = ny;
            }
        }

        dfs(depth + 1, coreCnt, lineCnt);
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            n = Integer.parseInt(br.readLine());

            map = new char[n + 2][n + 2];
            cores = new ArrayList<>();

            maxCore = 0;
            minLine = Integer.MAX_VALUE;

            for (int i = 0; i < n + 2; i++) {
                Arrays.fill(map[i], '#');
            }

            for (int i = 1; i <= n; i++) {
                StringTokenizer st = new StringTokenizer(br.readLine());

                for (int j = 1; j <= n; j++) {
                    map[i][j] = st.nextToken().charAt(0);

                    if (map[i][j] == '1' && i != 1 && i != n && j != 1 && j != n) {
                        cores.add(new Pair(i, j));
                    }
                }
            }

            dfs(0, 0, 0);

            System.out.println("#" + t + " " + minLine);
        }
    }
}