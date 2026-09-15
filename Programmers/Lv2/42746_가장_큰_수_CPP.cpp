#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool cmp(const string& a, const string& b){
    return a + b > b + a;
}

string solution(vector<int> numbers) {
    vector<string> s;
    s.reserve(numbers.size());

    for (int x : numbers) s.push_back(to_string(x));

    sort(s.begin(), s.end(), cmp);

    // "0"만 있는 경우 처리
    if (!s.empty() && s[0] == "0") return "0";

    string ans;
    for (const auto& t : s) ans += t;
    return ans;
}
