#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n = 0;
    int m = 0;
    int t = 0;
    int a = 0;
    int b = 0;
    int am = 0;
    int bm = 0;

    cin >> n >> m >> t;

    vector<vector<int>> arr;
    vector<vector<int>> cut;
    cut.resize(t, vector<int> (2));

    arr.resize(m, vector<int> (n));

    vector<int> arr1;
    vector<int> arr2;

    for(int i = 0; i < t; i++){
        cin >> cut[i][0] >> cut[i][1];
    }

    sort(cut.begin(), cut.end());
    // cout << '\n';
    // for(int i = 0; i < t; i++){
    //     if(cut[i][0] == 0){
    //         // cout  << a << '\n' ;
    //         am = max(m-a, a);
    //         a = cut[i][1];
    //     }
    //     if(cut[i][0] == 1){
    //         // cout << n << ' ' << b << '\n';
    //         bm = max(n-b, b);
    //         b = cut[i][1];
    //     }
    //     // cout << cut[i][0] << ' ' << cut[i][1] << '\n';
    // }
    int q = 0;
    for(int i = 0; i <= t; i++){
        if(i == t){
            q = m-a;
            // a = cut[i][1];
            arr1.push_back(q);
            // cout << cut[i][1] << ' ';
        
            q = n-b;
            // b = cut[i][1];
            arr2.push_back(q);
            break;
        }
        if(cut[i][0] == 0){
            q = cut[i][1]-a;
            a = cut[i][1];
            arr1.push_back(q);
            // cout << cut[i][1] << ' ';
        }
        if(cut[i][0] == 1){
            q = cut[i][1]-b;
            b = cut[i][1];
            arr2.push_back(q);
        }
    }

    for(int i = 0; i < arr1.size(); i++){
        am = max(am, arr1[i]);
        // cout << arr1[i] << ' ';
    }
    for(int i = 0; i < arr2.size(); i++){
        bm = max(bm, arr2[i]);
    }
    
    // cout << am  << ' ' << bm;
    cout << am * bm;

    return 0;
}