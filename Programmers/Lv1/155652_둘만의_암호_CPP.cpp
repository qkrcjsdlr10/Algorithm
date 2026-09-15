#include <string>
using namespace std;

string solution(string s, string skip, int index) {
    string answer = "";

    for (char a : s) {
        int cnt = 0;
        int cur = 0;

        while (cnt < index) {
            cur++; // 먼저 한 칸 이동
            char next = 'a' + ((a - 'a' + cur) % 26);

            if (skip.find(next) == string::npos) { // skip에 없으면 카운트
                cnt++;
            }
        }

        char c = 'a' + ((a - 'a' + cur) % 26);
        answer += c;
    }

    return answer;
}