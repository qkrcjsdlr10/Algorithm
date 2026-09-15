#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>

using namespace std;

vector<int> solution(string s) {
    vector<vector<int>> sets;
    vector<int> cur;
    string num = "";

    // 1) 문자열 파싱 → 집합들 추출
    for (int i = 0; i < s.size(); i++) {
        if ('0' <= s[i] && s[i] <= '9') {
            num += s[i];
        } else {
            if (!num.empty()) {
                cur.push_back(stoi(num));
                num.clear();
            }
            if (s[i] == '}') {
                if (!cur.empty()) {
                    sets.push_back(cur);
                    cur.clear();
                }
            }
        }
    }

    // 2) 집합 크기 기준 정렬
    sort(sets.begin(), sets.end(),
         [](const vector<int>& a, const vector<int>& b) {
             return a.size() < b.size();
         });

    // 3) 작은 집합부터 새 원소만 추가
    unordered_set<int> seen;
    vector<int> answer;

    for (auto& v : sets) {
        for (int x : v) {
            if (seen.insert(x).second) {
                answer.push_back(x);
                break;
            }
        }
    }

    return answer;
}
