import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.StringTokenizer;

public class Solution {
    public static void main(String[] args) throws IOException {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        int tc = 10;
        for(int t = 1; t <= tc; t++){
            int n = Integer.parseInt(br.readLine());
            String[] arr = new String[n + 1];
            boolean check = true;

            for(int i = 0; i < n; i++){
                StringTokenizer st = new StringTokenizer(br.readLine());
                int cur = Integer.parseInt(st.nextToken());
                String value = st.nextToken();
                boolean isNumber = value.chars().allMatch(Character::isDigit);

                int l = -1;
                int r = -1;

                if (st.hasMoreTokens()) {
                    l = Integer.parseInt(st.nextToken());
                }
                if (st.hasMoreTokens()) {
                    r = Integer.parseInt(st.nextToken());
                }


                boolean isLeaf = true;

                if(l != -1 || r != -1){
                    isLeaf = false;
                }

                if(isNumber){
                    if(!isLeaf){
                        check = false;
                    }
                }else{
                    if(isLeaf){
                        check = false;
                    }
                }
            }

            System.out.println("#" + t + " " + (check ? 1 : 0));
        }
    }
}
