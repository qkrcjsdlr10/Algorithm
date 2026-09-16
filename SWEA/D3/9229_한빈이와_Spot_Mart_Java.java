import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.StringTokenizer;

public class Solution {

    static int n = 0;
    static int target = 0;
    static int[] arr;
    static int mx = 0;
    static boolean[] vis;
    static ArrayList<Integer> temp;

    public static void dfs(int depth){
        if(temp.size() > 2) return;

        if(temp.size() == 2){
//            System.out.println(temp.get(0) + " " + temp.get(1));
            if(temp.get(0) + temp.get(1) > target) return;
            mx = Math.max(temp.get(0) + temp.get(1), mx);
            return;
        }

        for(int i = 0; i < n; i++){
            if(vis[i]) continue;
            vis[i] = true;
            temp.add(arr[i]);
            dfs(depth + 1);
            temp.remove(temp.size() - 1);
            vis[i] = false;
        }
    }

    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int tc = Integer.parseInt(br.readLine());
        for(int t = 1; t <= tc; t++){
            StringTokenizer st = new StringTokenizer(br.readLine());

            mx = 0;
            n = Integer.parseInt(st.nextToken());
            target = Integer.parseInt(st.nextToken());
            arr = new int[n];
            vis = new boolean[n];
            temp = new ArrayList<>();

            st = new StringTokenizer(br.readLine());

            for(int i = 0; i < n; i++){
                arr[i] = Integer.parseInt(st.nextToken());
            }

            dfs(0);

            System.out.println("#" + t + " " + ((mx == 0) ? -1 : mx));
        }

    }
}
