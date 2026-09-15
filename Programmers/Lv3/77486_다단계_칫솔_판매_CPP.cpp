#include <bits/stdc++.h>
using namespace std;

vector<int> solution(vector<string> enroll, vector<string> referral,
                     vector<string> seller, vector<int> amount) {
    int n = enroll.size();

    vector<int> money(n, 0);
    unordered_map<string, int> idx;
    unordered_map<string, string> parent;

    for (int i = 0; i < n; i++) {
        idx[enroll[i]] = i;
        parent[enroll[i]] = referral[i];
    }

    for (int i = 0; i < seller.size(); i++) {
        string name = seller[i];
        int price = amount[i] * 100;

        while (name != "-" && price > 0) {
            int give = price / 10;
            int keep = price - give;

            money[idx[name]] += keep;

            name = parent[name];
            price = give;
        }
    }

    return money;
}