#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

vector<int> solution(string msg) {
    unordered_map<string, int> dic;
    int nextIdx = 1;

    // 초기 사전: "A"~"Z" -> 1~26
    for (char c = 'A'; c <= 'Z'; ++c) {
        dic[string(1, c)] = nextIdx++;
    }

    vector<int> answer;
    int i = 0;

    while (i < (int)msg.size()) {
        string w(1, msg[i]);
        int len = 1;

        // w를 최대한 늘리기 (범위 체크!)
        while (i + len <= (int)msg.size()) {
            string cand = msg.substr(i, len);
            if (dic.count(cand)) {
                w = cand;
                len++;
            } else {
                break;
            }
        }

        // w 출력
        answer.push_back(dic[w]);

        // w + 다음글자 사전에 추가 (다음 글자 존재할 때만)
        if (i + (int)w.size() < (int)msg.size()) {
            string newEntry = w + msg[i + (int)w.size()];
            dic[newEntry] = nextIdx++;
        }

        // i 이동
        i += (int)w.size();
    }

    return answer;
}
