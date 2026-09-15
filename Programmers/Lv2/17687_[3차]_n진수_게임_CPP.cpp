#include <bits/stdc++.h>
using namespace std;

string toBase(int x, int n) {
    const string digits = "0123456789ABCDEF";
    if (x == 0) return "0";
    string s;
    while (x > 0) {
        s.push_back(digits[x % n]);
        x /= n;
    }
    reverse(s.begin(), s.end());
    return s;
}

string solution(int n, int t, int m, int p) {
    string stream = "";
    for (int num = 0; (int)stream.size() < t * m; num++) {
        stream += toBase(num, n);
    }

    string answer = "";
    for (int i = 0; (int)answer.size() < t; i++) {
        if (i % m == (p - 1)) {
            answer.push_back(stream[i]);
        }
    }
    return answer;
}
