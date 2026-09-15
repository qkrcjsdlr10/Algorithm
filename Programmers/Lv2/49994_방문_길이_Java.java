import java.util.*;

class Solution {
    
    public int solution(String dirs) {
        
        Set<String> visited = new HashSet<>();
        
        // 2배 확장 좌표
        int x = 0;
        int y = 0;
        
        int answer = 0;
        
        for(char dir : dirs.toCharArray()) {
            
            int dx = 0;
            int dy = 0;
            
            if(dir == 'U') dy = 2;
            if(dir == 'D') dy = -2;
            if(dir == 'R') dx = 2;
            if(dir == 'L') dx = -2;
            
            int nx = x + dx;
            int ny = y + dy;
            
            // 원래 범위 -5 ~ 5를 2배 확장
            if(nx < -10 || nx > 10 || ny < -10 || ny > 10) {
                continue;
            }
            
            // 중간 좌표
            int mx = x + dx / 2;
            int my = y + dy / 2;
            
            // 첫 번째 길
            String path1 = makePath(x, y, mx, my);
            String path2 = makePath(mx, my, nx, ny);
            
            if(!visited.contains(path1)) {
                visited.add(path1);
                visited.add(reverse(path1));
                answer++;
            }
            
            if(!visited.contains(path2)) {
                visited.add(path2);
                visited.add(reverse(path2));
                answer++;
            }
            
            x = nx;
            y = ny;
        }
        
        // 2배 확장했기 때문에 실제 길은 절반
        return answer / 2;
    }
    
    
    // 이동 경로 생성
    static String makePath(int x1, int y1, int x2, int y2) {
        return x1 + "," + y1 + "," + x2 + "," + y2;
    }
    
    
    // 반대 방향 경로
    static String reverse(String path) {
        String[] p = path.split(",");
        
        return p[2] + "," + p[3] + "," + p[0] + "," + p[1];
    }
}