#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    for (int tc = 1; tc <= 10; ++tc) {
        int n;
        cin >> n;

        list<int> passwords;

        for (int i = 0; i < n; ++i) {
            int num;
            cin >> num;
            passwords.push_back(num);
        }

        int commandCount;
        cin >> commandCount;

        while (commandCount--) {
            char command;
            cin >> command;

            int position, count;
            cin >> position >> count;

            auto cursor = passwords.begin();
            advance(cursor, position);

            while (count--) {
                int num;
                cin >> num;

                cursor = passwords.insert(cursor, num);
                ++cursor;
            }
        }

        cout << '#' << tc << ' ';

        auto cursor = passwords.begin();
        for (int i = 0; i < 10; ++i, ++cursor) {
            cout << *cursor << ' ';
        }

        cout << '\n';
    }

    return 0;
}