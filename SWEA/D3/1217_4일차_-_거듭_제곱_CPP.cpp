#include <iostream>
using namespace std;
int ress;

void func(int a, int b){
    if(b == 0){
        return;
    }

    ress = ress * a;
    // cout << ress << ' ';
    
    func(a, --b);
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    for(int tc = 1; tc <= 10; tc++){
        int testCase;
        cin >> testCase;
        int n, m;
        int res = 0;
        cin >> n >> m;
        res = n;
        ress = n;
    
        // for(int i = 0; i < m-1; i++){
        //     res = res * n;
        // }
        func(n, m-1);
    
        cout << '#' << testCase << ' ' << ress << '\n';
    }

    return 0;
}