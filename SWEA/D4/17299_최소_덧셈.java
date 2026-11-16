package swea;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class swea17299_최소_덧셈 {

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();
        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            String num = br.readLine();
            int res = Integer.MAX_VALUE;

            for (int i = 0; i < num.length()-1; i++) {
                int num1 = Integer.parseInt(num.substring(0, i + 1));
                int num2 = Integer.parseInt(num.substring(i + 1));
//                System.out.println(num1 + " " + num2);
                res = Math.min(res, num1 + num2);
            }

            sb.append("#").append(t).append(" ").append(res).append("\n");
        }
        System.out.println(sb);
    }

}
