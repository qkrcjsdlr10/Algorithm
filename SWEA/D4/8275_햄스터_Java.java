import java.io.*;
import java.util.*;

public class Solution {

    static int n, x, m;
    static int[] vis;
    static int[] answer;
    static int maxSum;

    static class Node {
        int start;
        int end;
        List<List<Integer>> arr = new ArrayList<>();

        Node(int start, int end) {
            this.start = start;
            this.end = end;
        }
    }

    public static void dfs(int depth, int[][] arr) {

        // 모든 조건을 처리함
        if(depth == m) {
            checkAnswer();
            return;
        }

        int start = arr[depth][0];
        int end = arr[depth][1];
        int mx = arr[depth][2];

        Node node = new Node(start, end);

        // 첫 번째 조건
        if(depth == 0) {
            makePerm(
                    0,
                    start,
                    end,
                    mx,
                    0,
                    new ArrayList<>(),
                    node
            );
        }

        // 두 번째 조건부터
        else {
            List<Integer> temp = new ArrayList<>();

            fillNextPerm(start, end, temp);

            makeNextPerm(
                    0,
                    mx,
                    0,
                    temp,
                    node
            );
        }

        // 현재 조건에서 가능한 후보 하나씩 적용
        for(List<Integer> row : node.arr) {

            int[] backup = vis.clone();

            for(int j = start; j <= end; j++) {
                vis[j] = row.get(j - start);
            }

            dfs(depth + 1, arr);

            // 백트래킹
            vis = backup;
        }
    }

    // 현재 vis를 이용해서 다음 조건의 초기 배열 생성
    public static void fillNextPerm(
            int start,
            int end,
            List<Integer> temp
    ) {

        for(int i = start; i <= end; i++) {
            temp.add(vis[i]);
        }
    }

    // 이미 값이 존재하는 위치는 고정하고
    // -1인 위치만 0 ~ x를 넣어봄
    public static void makeNextPerm(
            int depth,
            int sumMx,
            int sum,
            List<Integer> temp,
            Node node
    ) {

        // 이미 합 초과
        if(sum > sumMx) {
            return;
        }

        // 구간 끝까지 결정
        if(depth == temp.size()) {

            if(sum == sumMx) {
                node.arr.add(new ArrayList<>(temp));
            }

            return;
        }

        // 이전 조건에 의해 이미 정해진 값
        if(temp.get(depth) != -1) {

            makeNextPerm(
                    depth + 1,
                    sumMx,
                    sum + temp.get(depth),
                    temp,
                    node
            );

            return;
        }

        // 아직 정해지지 않은 위치
        for(int i = 0; i <= x; i++) {

            if(sum + i > sumMx) {
                break;
            }

            temp.set(depth, i);

            makeNextPerm(
                    depth + 1,
                    sumMx,
                    sum + i,
                    temp,
                    node
            );

            // 백트래킹
            temp.set(depth, -1);
        }
    }

    // 첫 번째 조건의 모든 경우 생성
    public static void makePerm(
            int depth,
            int start,
            int end,
            int sumMx,
            int sum,
            List<Integer> temp,
            Node node
    ) {

        if(sum > sumMx) {
            return;
        }

        // 마지막 칸은 남은 합으로 결정
        if(depth == end - start) {

            int endNum = sumMx - sum;

            if(endNum < 0 || endNum > x) {
                return;
            }

            temp.add(endNum);

            node.arr.add(new ArrayList<>(temp));

            temp.remove(temp.size() - 1);

            return;
        }

        for(int i = 0; i <= x; i++) {

            if(sum + i > sumMx) {
                break;
            }

            temp.add(i);

            makePerm(
                    depth + 1,
                    start,
                    end,
                    sumMx,
                    sum + i,
                    temp,
                    node
            );

            temp.remove(temp.size() - 1);
        }
    }

    // 모든 조건을 만족한 상태에서 최종 정답 결정
    public static void checkAnswer() {

        int[] cand = vis.clone();

        int sum = 0;

        for(int i = 1; i <= n; i++) {

            // 어떤 조건에도 포함되지 않은 우리는 최대 x
            if(cand[i] == -1) {
                cand[i] = x;
            }

            sum += cand[i];
        }

        // 햄스터 수가 더 많음
        if(sum > maxSum) {
            maxSum = sum;
            answer = cand;
        }

        // 햄스터 수가 같으면 사전순 비교
        else if(sum == maxSum && isFront(cand, answer)) {
            answer = cand;
        }
    }

    public static boolean isFront(int[] a, int[] b) {

        if(b == null) {
            return true;
        }

        for(int i = 1; i <= n; i++) {

            if(a[i] < b[i]) {
                return true;
            }

            if(a[i] > b[i]) {
                return false;
            }
        }

        return false;
    }

    public static void main(String[] args) throws IOException {

        BufferedReader br =
                new BufferedReader(new InputStreamReader(System.in));

        StringBuilder sb = new StringBuilder();

        int T = Integer.parseInt(br.readLine());

        for(int tc = 1; tc <= T; tc++) {

            StringTokenizer st =
                    new StringTokenizer(br.readLine());

            n = Integer.parseInt(st.nextToken());
            x = Integer.parseInt(st.nextToken());
            m = Integer.parseInt(st.nextToken());

            int[][] arr = new int[m][3];

            for(int i = 0; i < m; i++) {

                st = new StringTokenizer(br.readLine());

                arr[i][0] = Integer.parseInt(st.nextToken());
                arr[i][1] = Integer.parseInt(st.nextToken());
                arr[i][2] = Integer.parseInt(st.nextToken());
            }

            vis = new int[n + 1];
            Arrays.fill(vis, -1);

            answer = null;
            maxSum = -1;

            dfs(0, arr);

            sb.append("#").append(tc).append(" ");

            if(answer == null) {
                sb.append("-1");
            }
            else {
                for(int i = 1; i <= n; i++) {
                    sb.append(answer[i]).append(" ");
                }
            }

            sb.append("\n");
        }

        System.out.print(sb);
    }
}