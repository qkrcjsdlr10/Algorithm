import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Solution{

    static boolean[] vis1;
    static boolean[] vis2;
    static boolean[] vis3;
    static int n = 0;
    static int res = 0;

    static void dfs(int depth){
        if(depth == n){
            res++;
            return;
        }

        for (int col = 0; col < n; col++) {
            if(!isValid(depth, col)) continue;
            vis1[col] = vis2[col + depth] = vis3[depth - col + n - 1] = true;
            dfs(depth  + 1);
            vis1[col] = vis2[col + depth] = vis3[depth - col + n - 1] = false;
        }
    }

    static boolean isValid(int r, int c){
        return !vis1[c] && !vis2[c + r] && !vis3[r - c + n - 1];
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int tc = Integer.parseInt(br.readLine());
        for (int t = 1; t <= tc; t++) {
            n = Integer.parseInt(br.readLine());
            res = 0;
            
            vis1 = new boolean[n];
            vis2 = new boolean[n * 2 - 1];
            vis3 = new boolean[n * 2 - 1];

            dfs(0);

            System.out.println("#" + t + " " +res);
        }


    }
}
