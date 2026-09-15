#include <string>
#include <vector>
#include <iostream>

using namespace std;

int solution(int n, int w, int num) {
    int answer = 0;
    if(w == 1) return n - num + 1;

    int line = 1;
    int totalline = ((n + w - 1) / w);

    for(int i = w; i <= w * totalline; i += w){
        if(i < num){
            line++;
        }else{
            break;
        }
    }

    int pos = (num - 1) % w;   // 0-index 위치

    // line은 1부터 시작하므로, 짝수 line이 오른쪽->왼쪽 줄
    if(line % 2 == 0){
        pos = w - 1 - pos;
    }

    int lastCnt = n % w;
    if(lastCnt == 0) return totalline - line + 1;

    if(totalline % 2 == 1){
        // 마지막 줄이 왼쪽부터 채워짐
        if(pos < lastCnt) answer = totalline - line + 1;
        else answer = totalline - line;
    }else{
        // 마지막 줄이 오른쪽부터 채워짐
        if(pos >= w - lastCnt) answer = totalline - line + 1;
        else answer = totalline - line;
    }

    return answer;
}