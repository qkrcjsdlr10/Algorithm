#include <bits/stdc++.h>
using namespace std;


int n = 0;
int mx = 0;
set<pair<int, int>> st;
vector<int> arr;

void dfs(int depth){
    int a = 0;
    int b = 1;
    for(int i = arr.size() - 1; i >= 0; i--){
        // cout << arr[i] << ' ';
        a += b * arr[i];
        b *= 10;
    }
    // cout << '\n';
    // cout << a << ' ';

    if(st.find({a, depth}) == st.end()){
        st.insert({a, depth});
    }else{
        return;
    }

    if(depth == n){
        mx = max(a, mx);
        return;
    }

    // cout << "ddd";
    for(int i = 0; i < arr.size() - 1; i++){
        for(int j = i + 1; j < arr.size(); j++){
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            dfs(depth + 1);
            temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
}

void divnum(int num){
    while(num > 0){
        int div = 0;
        div = num % 10;
        arr.push_back(div);
        num /= 10;
    }
}

void swaparr(){
    int end = arr.size() - 1;
    int start = 0;
    while(start < end){
        // cout << "asdf";
        int temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int tc = 0;
    cin >> tc;

    for(int t = 0; t < tc; t++){
        mx = 0;
        st.clear();
        int num = 0;
        cin >> num >> n;

        arr.assign(0, 0);

        divnum(num);
        swaparr();

        // for(auto a : arr){
        //     cout << a << ' ';
        // }

        dfs(0);

        cout << '#' << t+1 << ' ' << mx << '\n';

    }
    

    return 0;
}