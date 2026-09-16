#include <bits/stdc++.h>
using namespace std;

int main() {
    for (int tc = 1; tc <= 10; tc++) {
        int dump;
        cin >> dump;

        multiset<int> s;

        for (int i = 0; i < 100; i++) {
            int x;
            cin >> x;
            s.insert(x);
        }

        while (dump--) {
            int mn = *s.begin();
            int mx = *prev(s.end());

            s.erase(s.begin());
            s.erase(prev(s.end()));

            s.insert(mn + 1);
            s.insert(mx - 1);
        }

        cout << "#" << tc << " "
             << *prev(s.end()) - *s.begin() << '\n';
    }
}