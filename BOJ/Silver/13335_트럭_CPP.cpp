#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, w, l;
    cin >> n >> w >> l;

    vector<int> arr(n);
    for (int i = 0; i < n; i++) cin >> arr[i];

    queue<pair<int,int>> bridge;
    int time = 0;
    int weight = 0;
    int idx = 0;
    int finished = 0;

    while (finished < n) {
        time++;

        int sz = bridge.size();
        for (int i = 0; i < sz; i++) {
            auto t = bridge.front();
            bridge.pop();
            t.second++;
            bridge.push(t);
        }

        if (!bridge.empty() && bridge.front().second >= w) {
            weight -= bridge.front().first;
            bridge.pop();
            finished++;
        }

        if (idx < n && weight + arr[idx] <= l) {
            bridge.push({arr[idx], 0});
            weight += arr[idx];
            idx++;
        }
    }

    cout << time;
    return 0;
}
