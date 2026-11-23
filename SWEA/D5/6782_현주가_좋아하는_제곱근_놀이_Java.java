package hw;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class swea6782_현주가_좋아하는_제곱근_놀이 {

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuffer sb = new StringBuffer();
        int tc = Integer.parseInt(br.readLine());
        for (int t = 1; t <= tc; t++) {
            long  n = Long.parseLong(br.readLine());
            long  res = 0;

            while(n != 2){
                long r = (long) Math.sqrt(n);
                if (r * r == n) {
                    n = r;
                    res++;
                } else {
                    long next = (r + 1) * (r + 1);
                    res += next - n;
                    n = next;
                }
            }

            sb.append("#").append(t).append(" ").append(res).append("\n");
        }

        System.out.println(sb);
    }

}
