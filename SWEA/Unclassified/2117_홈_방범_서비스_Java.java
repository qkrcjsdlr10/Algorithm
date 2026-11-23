package hw;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class swea2117_홈_방범_서비스 {

    static int n;
    static int m;

    public static boolean manDist(int y, int x, int curY, int curX, int k){
        return Math.abs(y - curY) + Math.abs(x - curX) < k;
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        StringBuilder sb = new StringBuilder();

        int tc = Integer.parseInt(br.readLine());

        for (int t = 1; t <= tc; t++) {
            StringTokenizer st = new StringTokenizer(br.readLine());

            int answer = 0;

            n = Integer.parseInt(st.nextToken());
            m = Integer.parseInt(st.nextToken());

            int[][] board = new int[n + 1][n + 1];

            for (int i = 0; i < n; i++) {
                st = new StringTokenizer(br.readLine());
                for (int j = 0; j < n; j++) {
                    board[i][j] = Integer.parseInt(st.nextToken());
                }
            }

            for(int k = 1; k <= 2 * n - 1; k++){
                for(int i = 0; i < n; i++){
                    for(int j = 0; j < n; j++){
                        int house = 0;
                        int area = 0;
                        for(int y = i - k + 1; y < i + k; y++){
                            for(int x = j - k + 1; x < j + k; x++){
                                if (y < 0 || y >= n || x < 0 || x >= n) continue;
                                if(manDist(y, x, i, j, k)){
                                    area++;
                                    if(board[y][x] == 1){
                                        house++;
                                    }
                                }
                            }
                        }
                        int cost = k * k + (k - 1) * (k - 1);
                        if (house * m >= cost) {
                            answer = Math.max(answer, house);
                        }
                    }

                }
            }

            sb.append("#").append(t).append(" ").append(answer).append("\n");
        }


        System.out.println(sb);

    }

}
