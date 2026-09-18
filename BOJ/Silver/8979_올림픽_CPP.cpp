#include <bits/stdc++.h>
using namespace std;

class Country{
    public:
    int id, gold, silver, bronze;
    Country(int id, int g, int s, int b)
        : id(id), gold(g), silver(s), bronze(b) {}

    static bool compare(const Country &a, const Country &b){
        if(a.gold != b.gold) return a.gold > b.gold;
        if(a.silver != b.silver) return a.silver > b.silver;
        if(a.bronze != b.bronze) return a.bronze > b.bronze;
        return a.bronze > b.bronze;
    }
};

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int t = 0;
    int target = 0;
    cin >> t >> target;

    vector<Country> v;

    for(int i = 0; i < t; i++){
        int id, g, s, b;
        cin >> id >> g >> s >> b;

        v.emplace_back(id, g, s, b);
    }

    sort(v.begin(), v.end(), Country::compare);

    int rank = 1;
    for (int i = 0; i < t; i++) {
        if (i > 0) {
            if (v[i].gold != v[i-1].gold ||
                v[i].silver != v[i-1].silver ||
                v[i].bronze != v[i-1].bronze)
                rank = i + 1;
        }

        if (v[i].id == target) {
            cout << rank;
            break;
        }
    }
    

    return 0;
}