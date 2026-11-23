package ws;

import java.io.*;
import java.util.*;

public class swea2477_차량_정비소 {

    static class Person {
        int index, time, aNum, bNum;

        Person(int index, int time) {
            this.index = index;
            this.time = time;
        }
    }

    static class Counter {
        int endTime = -1;
        int index = -1;
        int t;

        Counter(int t) {
            this.t = t;
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int T = Integer.parseInt(br.readLine());

        for (int tc = 1; tc <= T; tc++) {
            StringTokenizer st = new StringTokenizer(br.readLine());

            int N = Integer.parseInt(st.nextToken());
            int M = Integer.parseInt(st.nextToken());
            int K = Integer.parseInt(st.nextToken());
            int A = Integer.parseInt(st.nextToken());
            int B = Integer.parseInt(st.nextToken());

            Counter[] aList = new Counter[N];
            Counter[] bList = new Counter[M];
            Person[] persons = new Person[K + 1];

            ArrayDeque<Person> totalWait = new ArrayDeque<>();
            ArrayDeque<Person> aWait = new ArrayDeque<>();
            ArrayDeque<Person> bWait = new ArrayDeque<>();

            st = new StringTokenizer(br.readLine());
            for (int i = 0; i < N; i++) {
                aList[i] = new Counter(Integer.parseInt(st.nextToken()));
            }

            st = new StringTokenizer(br.readLine());
            for (int i = 0; i < M; i++) {
                bList[i] = new Counter(Integer.parseInt(st.nextToken()));
            }

            st = new StringTokenizer(br.readLine());
            for (int i = 1; i <= K; i++) {
                persons[i] = new Person(i, Integer.parseInt(st.nextToken()));
                totalWait.addLast(persons[i]);
            }

            int time = 0;
            int completed = 0;

            while (completed < K) {

                // 1. 접수 완료 고객을 정비 대기 큐로 이동
                for (int i = 0; i < N; i++) {
                    Counter counter = aList[i];

                    if (counter.endTime != time) continue;

                    bWait.addLast(persons[counter.index]);
                    counter.endTime = -1;
                    counter.index = -1;
                }

                // 2. 정비 완료 고객 처리
                for (int i = 0; i < M; i++) {
                    Counter counter = bList[i];

                    if (counter.endTime != time) continue;

                    completed++;
                    counter.endTime = -1;
                    counter.index = -1;
                }

                // 3. 현재 시간까지 도착한 고객을 접수 대기 큐에 추가
                while (!totalWait.isEmpty()
                        && totalWait.peekFirst().time <= time) {
                    aWait.addLast(totalWait.pollFirst());
                }

                // 4. 접수 대기 고객을 빈 접수 창구에 배정
                for (int i = 0; i < N; i++) {
                    if (aWait.isEmpty()) break;

                    Counter counter = aList[i];
                    if (counter.endTime != -1) continue;

                    Person p = aWait.pollFirst();

                    counter.endTime = time + counter.t;
                    counter.index = p.index;
                    p.aNum = i + 1;
                }

                // 5. 정비 대기 고객을 빈 정비 창구에 배정
                for (int i = 0; i < M; i++) {
                    if (bWait.isEmpty()) break;

                    Counter counter = bList[i];
                    if (counter.endTime != -1) continue;

                    Person p = bWait.pollFirst();

                    counter.endTime = time + counter.t;
                    counter.index = p.index;
                    p.bNum = i + 1;
                }

                time++;
            }

            // 6. 조건에 해당하는 고객 번호 합산
            int res = 0;

            for (int i = 1; i <= K; i++) {
                Person p = persons[i];

                if (p.aNum == A && p.bNum == B) {
                    res += p.index;
                }
            }

            sb.append("#").append(tc).append(" ")
                    .append(res == 0 ? -1 : res).append("\n");
        }

        System.out.print(sb);
    }
}