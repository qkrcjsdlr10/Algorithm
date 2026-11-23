package hw;

import java.io.*;
import java.util.*;

public class swea5648_원자_소멸_시뮬레이션 {

    static final int SIZE = 4001;

    static int[] dx = {0, 0, -1, 1};
    static int[] dy = {1, -1, 0, 0};

    // TC마다 새로 만들지 않음
    static int[][] board = new int[SIZE][SIZE];

    static class Node {
        int y, x, d, p;

        Node(int y, int x, int d, int p) {
            this.y = y;
            this.x = x;
            this.d = d;
            this.p = p;
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            int n = Integer.parseInt(br.readLine());
            int res = 0;

            ArrayDeque<Node> q = new ArrayDeque<>();
            ArrayDeque<Node> next = new ArrayDeque<>();

            // 한 턴에 방문 가능한 좌표는 최대 n개
            int[] pos = new int[n];

            for (int i = 0; i < n; i++) {
                StringTokenizer st = new StringTokenizer(br.readLine());

                int x = Integer.parseInt(st.nextToken()) * 2 + 2000;
                int y = Integer.parseInt(st.nextToken()) * 2 + 2000;
                int d = Integer.parseInt(st.nextToken());
                int p = Integer.parseInt(st.nextToken());

                q.addLast(new Node(y, x, d, p));
            }

            while (!q.isEmpty()) {
                int pcnt = 0;

                // 1. 모든 원자 이동
                while (!q.isEmpty()) {
                    Node cur = q.pollFirst();

                    int ny = cur.y + dy[cur.d];
                    int nx = cur.x + dx[cur.d];

                    if (ny < 0 || ny >= SIZE || nx < 0 || nx >= SIZE) {
                        continue;
                    }

                    cur.y = ny;
                    cur.x = nx;

                    // 처음 방문한 좌표만 기록
                    if (board[ny][nx] == 0) {
                        pos[pcnt++] = ny * SIZE + nx;
                    }

                    board[ny][nx]++;

                    next.addLast(cur);
                }

                // 2. 충돌 확인
                while (!next.isEmpty()) {
                    Node cur = next.pollFirst();

                    if (board[cur.y][cur.x] == 1) {
                        q.addLast(cur);
                    } else {
                        res += cur.p;
                    }
                }

                // 3. 이번 턴에 사용한 칸만 초기화
                for (int i = 0; i < pcnt; i++) {
                    int key = pos[i];

                    int y = key / SIZE;
                    int x = key % SIZE;

                    board[y][x] = 0;
                }
            }

            sb.append("#")
                    .append(t)
                    .append(" ")
                    .append(res)
                    .append('\n');
        }

        System.out.print(sb);
    }
}