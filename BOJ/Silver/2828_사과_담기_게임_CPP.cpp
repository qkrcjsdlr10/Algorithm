#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int m = 0;
    int j = 0;
    int res = 0;

    int lpos = 0;
    int rpos = 0;

    cin >> n >> m >> j;

    rpos = m - 1;

    for(int i = 0; i < j; i++){
        int a = 0;
        cin >> a;
        a--;
        if(a > rpos){
            res += a - rpos;
            lpos += a - rpos;
            rpos = a;
        }else if(a < lpos){
            res += lpos - a;
            rpos -= lpos - a;
            lpos = a;
        }
    }

    cout << res;

    return 0;
}
