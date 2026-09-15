#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

string solution(vector<string> participant, vector<string> completion) {
    unordered_map<string, int> m;

    for (auto &p : participant) m[p]++;   // 참가자 +1
    for (auto &c : completion) m[c]--;    // 완주자 -1

    for (auto &p : participant) {
        if (m[p] > 0) return p;           // 남은 사람
    }
    return "";
}