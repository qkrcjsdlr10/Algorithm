import java.util.*;

class Solution {
    
    public long solution(int n) {
        long answer = 0;
        
        List<Long> arr = new ArrayList<>();
        
        // System.out.println(arr.size());
        arr.add(0L);
        arr.add(1L);
        arr.add(2L);
        
        for(int i = 3; i <= n; i++){
            long num = (arr.get(i - 1) + arr.get(i - 2)) % 1234567;
            arr.add(num);
        }
        
        for(int i = 1; i <= n; i++){
            System.out.print(arr.get(i) + " ");
        }
        
        answer = arr.get(n);
        
        return answer;
    }
}