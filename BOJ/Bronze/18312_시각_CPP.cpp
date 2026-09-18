#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, k;
    cin >> n >> k;

    int res = 0;

    for (int h = 0; h <= n; h++) {
        for (int m = 0; m < 60; m++) {
            for (int s = 0; s < 60; s++) {
                
                int hh = h, mm = m, ss = s;
                bool ok = false;

                if (hh / 10 == k || hh % 10 == k) ok = true;

                if (mm / 10 == k || mm % 10 == k) ok = true;

                if (ss / 10 == k || ss % 10 == k) ok = true;

                if (ok) res++;
            }
        }
    }

    cout << res;
}
