#include <bits/stdc++.h>
using namespace std;

vector<int> arr, prt;
int n, k;

void dfs(int depth, int start) {
    if (depth == k) {
        for (int x : prt) cout << x << ' ';
        cout << '\n';
        return;
    }

    int previous = -1e9 - 1;

    for (int i = start; i < n; i++) {
        if (arr[i] == previous) continue;

        prt.push_back(arr[i]);
        previous = arr[i];

        dfs(depth + 1, i + 1);

        prt.pop_back();
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> k;
    arr.resize(n);

    for (int i = 0; i < n; i++) cin >> arr[i];
    sort(arr.begin(), arr.end());

    dfs(0, 0);
}