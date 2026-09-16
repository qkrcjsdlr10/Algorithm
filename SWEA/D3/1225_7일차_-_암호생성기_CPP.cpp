#include <bits/stdc++.h>
using namespace std;

list<int> myList;
vector<int> arr;


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    int n = 0;

    for(int tc = 0; tc < 10; tc++){
        cin >> n;

        arr.resize(8, 0);

        for(int i = 0; i < 8; i++){
            int a = 0;
            cin >> arr[i];
        }

        
        int idx = 0;
        int b = 1;

        while(1){
            arr[0] -= b;
            if(arr[0] <= 0){
                arr[0] = 0;
                arr.push_back(arr[0]);
                arr.erase(arr.begin());
                break;
            }
            arr.push_back(arr[0]);
            arr.erase(arr.begin());
            b = (b % 5) + 1;
        }

        cout << '#' << n;
        for(int i = 0; i < 8; i++){
            cout << ' ' << arr[i] << ' ';
        }
        cout << '\n';
        arr.resize(arr.size(), 0);
    }

    

    return 0;
}