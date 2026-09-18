#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int a = 0;
    int b = 0;
    string na = "";
    string nb = "";
    string sum = "";

    vector<string> alphabet = {
        "A", "B", "C", "D", "E", "F", "G",
        "H", "I", "J", "K", "L", "M", "N",
        "O", "P", "Q", "R", "S", "T", "U",
        "V", "W", "X", "Y", "Z"
    };

    vector<int> vec = {
        3, 2, 1, 2, 4, 3, 1, 3, 1, 1, 3, 1, 3,
        2, 1, 2, 2, 2, 1, 2, 1, 1, 1, 2, 2, 1
    };


    cin >> a >> b;
    cin >> na >> nb;
    int len = max(a, b);

    for (int i = 0; i < len; i++) {
        if (i < a) sum += na[i];
        if (i < b) sum += nb[i];
    }

    // cout << sum;
    vector<int> res;

    for(int i = 0; i+1 < sum.size(); i++){
        string sub = "";
        int num = 0;
        sub += sum[i];
        if (i + 1 < sum.size()) sub += sum[i + 1];

        for(int j = 0; j < alphabet.size(); j++){
            if(sub[0] == alphabet[j][0]){
                num += vec[j];
            }
            if(sub[1] == alphabet[j][0]){
                num += vec[j];
            }
        }

        if(num > 9){
            num = num%10;
        }
        res.push_back(num);
    }

    while(res.size() > 2){
        vector<int> temp;
        for(int i = 0; i+1 < res.size(); i++){
            int itemp = res[i] + res[i+1];
            if(itemp > 9){
                itemp = itemp % 10;
            }
            temp.push_back(itemp);
        }
        res = temp;
    }

    if(res[0] == 0){
        cout << res[1] << '%';
    }else{
        cout << res[0] << res[1] << '%';
    }
    

    return 0;
}