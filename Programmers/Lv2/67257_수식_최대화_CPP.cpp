#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<char> op;
vector<char> tmp;
vector<bool> vis;
vector<string> order;

void dfs() {
    if (tmp.size() == op.size()) {
        string s = "";
        for (auto c : tmp) s.push_back(c);
        order.push_back(s);
        return;
    }

    for (int i = 0; i < op.size(); i++) {
        if (vis[i]) continue;
        tmp.push_back(op[i]);
        vis[i] = true;
        dfs();
        tmp.pop_back();
        vis[i] = false;
    }
}

long long solution(string expression) {
    long long ans = 0;

    set<char> st;
    string ops = "";

    vector<long long> num;
    string cur = "";

    for (auto c : expression) {
        if (c < '0' || c > '9') {
            st.insert(c);
            ops.push_back(c);
            num.push_back(stoll(cur));
            cur = "";
        } else {
            cur += c;
        }
    }
    num.push_back(stoll(cur));

    for (auto c : st) op.push_back(c);

    vis.assign(op.size(), false);
    dfs();

    for (auto &ord : order) {
        vector<long long> nums = num;
        string opers = ops;

        for (int i = 0; i < ord.size(); i++) {
            for (int j = 0; j < opers.size();) {
                if (opers[j] == ord[i]) {
                    long long res;
                    if (opers[j] == '+') res = nums[j] + nums[j + 1];
                    else if (opers[j] == '-') res = nums[j] - nums[j + 1];
                    else res = nums[j] * nums[j + 1];

                    nums[j] = res;
                    nums.erase(nums.begin() + j + 1);
                    opers.erase(opers.begin() + j);
                } else {
                    j++;
                }
            }
        }

        ans = max(ans, llabs(nums[0]));
    }

    return ans;
}