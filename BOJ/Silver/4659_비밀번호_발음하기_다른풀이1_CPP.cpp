#include <bits/stdc++.h>
using namespace std;

bool is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

bool is_acceptable(const string& s) {
    bool has_vowel = false;
    int vowel_streak = 0;
    int consonant_streak = 0;

    for (int i = 0; i < s.size(); i++) {
        if (is_vowel(s[i])) {
            has_vowel = true;
            vowel_streak++;
            consonant_streak = 0;
        } else {
            consonant_streak++;
            vowel_streak = 0;
        }

        // 3 모음 or 3 자음 연속
        if (vowel_streak == 3 || consonant_streak == 3)
            return false;

        // 같은 글자 두 번 연속 (ee, oo 제외)
        if (i > 0 && s[i] == s[i - 1] && s[i] != 'e' && s[i] != 'o')
            return false;
    }

    return has_vowel;
}

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    string s;
    while (cin >> s) {
        if (s == "end") break;

        if (is_acceptable(s)) {
            cout << '<' << s << "> is acceptable.\n";
        } else {
            cout << '<' << s << "> is not acceptable.\n";
        }
    }

    return 0;
}
