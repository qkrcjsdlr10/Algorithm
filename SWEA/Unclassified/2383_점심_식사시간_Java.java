
package other;

import java.io.*;
import java.util.*;

public class swea2383_점심_식사시간 {

    static int res;
    static ArrayList<Pair> q;

    static class Step {
        int x, y, k;
        ArrayDeque<Integer> wq, dq;

        Step(int y, int x, int k) {
            this.y = y;
            this.x = x;
            this.k = k;
            this.wq = new ArrayDeque<>();
            this.dq = new ArrayDeque<>();
        }

        Step(Step other) {
            this(other.y, other.x, other.k);
        }
    }

    static class Pair {
        int first, second;

        Pair(int first, int second) {
            this.first = first;
            this.second = second;
        }
    }

    static int calDist(int ay, int ax, int by, int bx) {
        return Math.abs(ay - by) + Math.abs(ax - bx);
    }

    // 한 계단의 시간 시뮬레이션
    static int simulateStep(Step step, int time, boolean[] selected, boolean[] vis, boolean target) {
        int finish = 0;

        // 1. 계단을 다 내려간 사람 제거
        while (!step.dq.isEmpty() && step.dq.peekFirst() <= time) {
            step.dq.pollFirst();
            finish++;
        }

        // 2. 기존 대기자 먼저 입장
        while (!step.wq.isEmpty() && step.dq.size() < 3) {
            step.wq.pollFirst();
            step.dq.addLast(time + step.k);
        }

        // 3. 새로 도착한 사람 처리
        for (int i = 0; i < q.size(); i++) {
            if (selected[i] != target || vis[i]) continue;

            Pair p = q.get(i);
            int dist = calDist(p.first, p.second, step.y, step.x);

            if (dist < time) {
                if (step.dq.size() < 3) {
                    step.dq.addLast(time + step.k);
                } else {
                    step.wq.addLast(time);
                }
                vis[i] = true;
            }
        }

        return finish;
    }

    // 현재 계단 배정의 완료 시간 계산
    static void func(ArrayList<Step> qList, ArrayList<Integer> temp, int pCnt) {

        boolean[] selected = new boolean[pCnt];
        boolean[] vis = new boolean[pCnt];

        for (int num : temp) {
            selected[num - 1] = true;
        }

        Step step1 = new Step(qList.get(0));
        Step step2 = new Step(qList.get(1));

        int finish = 0;
        int time = 0;

        while (finish < pCnt) {
            finish += simulateStep(step1, time, selected, vis, true);
            finish += simulateStep(step2, time, selected, vis, false);
            time++;
        }

        res = Math.min(res, time - 1);
    }

    // 부분집합 탐색
    static void dfs(ArrayList<Step> qList, int start, int pCnt, ArrayList<Integer> temp) {

        func(qList, temp, pCnt);

        for (int i = start; i <= pCnt; i++) {
            temp.add(i);
            dfs(qList, i + 1, pCnt, temp);
            temp.remove(temp.size() - 1);
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(
                new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            res = Integer.MAX_VALUE;

            int n = Integer.parseInt(br.readLine());

            ArrayList<Step> qList = new ArrayList<>();
            q = new ArrayList<>();

            for (int i = 0; i < n; i++) {
                StringTokenizer st = new StringTokenizer(br.readLine());

                for (int j = 0; j < n; j++) {
                    int value = Integer.parseInt(st.nextToken());

                    if (value == 1) {
                        q.add(new Pair(i, j));
                    } else if (value > 1) {
                        qList.add(new Step(i, j, value));
                    }
                }
            }

            dfs(qList, 1, q.size(), new ArrayList<>());

            sb.append("#").append(t).append(" ").append(res).append("\n");
        }

        System.out.print(sb);
    }
}
