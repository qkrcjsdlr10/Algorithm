#include <string>
#include <vector>
using namespace std;

string solution(string number, int k) {
    string st;
    st.reserve(number.size());

    for (char c : number) {
        while (!st.empty() && k > 0 && st.back() < c) {
            st.pop_back();
            k--;
        }
        st.push_back(c);
    }

    // 아직 k가 남아있으면 뒤에서 제거
    if (k > 0) st.erase(st.end() - k, st.end());

    return st;
}
