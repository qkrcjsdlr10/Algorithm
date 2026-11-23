package ws;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class swea3289_서로소_집합 {

    static int[] p;

    public static int find(int x){
        if(p[x] == x) return x;
        return p[x] = find(p[x]);
    }

    public static void union(int a, int b){
        a = find(a);
        b = find(b);

        if(a == b) return;
        p[b] = a;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            sb.append("#").append(t).append(" ");
            StringTokenizer st = new StringTokenizer(br.readLine());

            int n = Integer.parseInt(st.nextToken());
            int e = Integer.parseInt(st.nextToken());
            p = new int[n + 1];

            for (int i = 1; i <= n; i++) {
                p[i] = i;
            }

            for (int i = 0; i < e; i++) {
                st = new StringTokenizer(br.readLine());
                int cmd = Integer.parseInt(st.nextToken());
                int a = Integer.parseInt(st.nextToken());
                int b = Integer.parseInt(st.nextToken());

                if(cmd == 0){
                    union(a, b);
                }else{
                    char temp = ' ';
                    if(find(a) == find(b)){
                        temp = '1';
                    }else{
                        temp = '0';
                    }
                    sb.append(temp);
                }
            }

            sb.append("\n");
        }



        System.out.println(sb);
    }

}
