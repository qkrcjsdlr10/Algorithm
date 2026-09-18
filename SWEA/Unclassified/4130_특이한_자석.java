package ws;

import java.io.*;
import java.util.*;

public class swea4130_특이한_자석 {

    static List<Integer>[] gears = new ArrayList[4];

    // d = 1 시계방향, -1 반시계방향
    // dir = 'r' 오른쪽 전파, 'l' 왼쪽 전파
    static void dfs(int cur, int d, char dir) {
        if(cur < 0 || cur > 3) return;

        if(dir == 'r') {
            // 오른쪽 자석 확인
            if(cur < 3) {
                if(gears[cur].get(2) != gears[cur + 1].get(6)) {
                    dfs(cur + 1, -d, 'r');
                }
            }

        } else {
            // 왼쪽 자석 확인
            if(cur > 0) {
                if(gears[cur].get(6) != gears[cur - 1].get(2)) {
                    dfs(cur - 1, -d, 'l');
                }
            }
        }

        // 리턴하면서 현재 자석 회전
        rotate(cur, d);
    }

    static void rotate(int cur, int d) {
        // 시계방향
        if(d == 1) {
            int x = gears[cur].remove(7);
            gears[cur].add(0, x);
        }

        // 반시계방향
        else {
            int x = gears[cur].remove(0);
            gears[cur].add(x);
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for(int t = 1; t <= tc; t++) {
            int k = Integer.parseInt(br.readLine());
            gears = new ArrayList[4];

            for(int i = 0; i < 4; i++) {
                gears[i] = new ArrayList<>();
                StringTokenizer st = new StringTokenizer(br.readLine());

                for(int j = 0; j < 8; j++) {
                    gears[i].add(Integer.parseInt(st.nextToken()));
                }
            }

            for(int i = 0; i < k; i++) {
                StringTokenizer st = new StringTokenizer(br.readLine());

                int cur = Integer.parseInt(st.nextToken()) - 1;
                int d = Integer.parseInt(st.nextToken());

                // 회전하기 전에 양쪽 전파 여부 확인
                boolean left = false;
                boolean right = false;

                if(cur > 0) {
                    if(gears[cur].get(6) != gears[cur - 1].get(2)) {
                        left = true;
                    }
                }

                if(cur < 3) {
                    if(gears[cur].get(2) != gears[cur + 1].get(6)) {
                        right = true;
                    }
                }

                // 왼쪽 전파
                if(left) {
                    dfs(cur - 1, -d, 'l');
                }

                // 오른쪽 전파
                if(right) {
                    dfs(cur + 1, -d, 'r');
                }

                // 시작 자석은 마지막에 한 번만 회전
                rotate(cur, d);
            }

            int res = 0;

            if(gears[0].get(0) == 1) res += 1;
            if(gears[1].get(0) == 1) res += 2;
            if(gears[2].get(0) == 1) res += 4;
            if(gears[3].get(0) == 1) res += 8;

            sb.append("#").append(t).append(" ").append(res).append("\n");
        }

        System.out.print(sb);
    }
}