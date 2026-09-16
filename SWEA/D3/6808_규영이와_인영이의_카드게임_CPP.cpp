#include <bits/stdc++.h>
using namespace std;

vector<int> gyu, in;
int visited[9];
int win, lose;
int fact[10];

void dfs(int depth, int gyuScore, int inScore) {
    if (gyuScore > 85) {
        win += fact[9 - depth];
        return;
    }

    if (inScore > 85) {
        lose += fact[9 - depth];
        return;
    }

    if (depth == 9) {
        if (gyuScore > inScore) win++;
        else lose++;
        return;
    }

    for (int i = 0; i < 9; i++) {
        if (visited[i]) continue;

        visited[i] = 1;

        int sum = gyu[depth] + in[i];

        if (gyu[depth] > in[i])
            dfs(depth + 1, gyuScore + sum, inScore);
        else
            dfs(depth + 1, gyuScore, inScore + sum);

        visited[i] = 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    fact[0] = 1;
    for (int i = 1; i <= 9; i++) {
        fact[i] = fact[i - 1] * i;
    }

    int T;
    cin >> T;

    for (int tc = 1; tc <= T; tc++) {
        gyu.clear();
        in.clear();
        memset(visited, 0, sizeof(visited));
        win = lose = 0;

        vector<int> used(19, 0);

        for (int i = 0; i < 9; i++) {
            int x;
            cin >> x;
            gyu.push_back(x);
            used[x] = 1;
        }

        for (int i = 1; i <= 18; i++) {
            if (!used[i]) in.push_back(i);
        }

        dfs(0, 0, 0);

        cout << "#" << tc << ' ' << win << ' ' << lose << '\n';
    }

    return 0;
}