#include <bits/stdc++.h>
using namespace std;


//abcdefghijklmnopqrstuvwxyz
//01234567890123456789012345
//0 4 8 14 20
int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    vector<vector<int>> arr1;
    
    arr1.resize(3, vector<int> (3));

    int c[5] = {0, 4, 8, 14, 20};
    while(1){
        bool check = true;
        int arr[26] = {};
        string str = "";
        string s = "";
        cin >> s;

        if(s == "end"){
            break;
        }
    
    
        if(s.size() == 1){
            for(int i = 0; i < 5; i++){
                if(s[0] - 'a' == c[i]){
                    check = false;
                }
            }
            if(check == false){
                cout << '<' << s << '>' << " is acceptable." << '\n';
            }else{
                cout << '<' << s << '>' << " is not acceptable." << '\n';
            }
            continue;
        }
    
        for(int i = 0; i < s.size(); i++){
            int a = s[i] - 'a';
            arr[a]++;
        }
    
        if(!arr[0] && !arr[4] && !arr[8] && !arr[14] && !arr[20])
        {
            check = false;
        }

        if(check == true){
            for(int i = 0; i < s.size() - 1; i++){
                string a = "";
                for(int j = i; j < i + 2; j++){
                    a += s[j];
                }
                if(a[0] == a[1]){
                    if(a[0] != 'e' && a[0] != 'o'){
                        check = false;
                        break;
                    }
                }
            }
        
            if(s.size() >= 3){
                for(int i = 0; i <= s.size() - 3; i++){
                    string a = "";
                    for(int j = i; j < i + 3; j++){
                        a += s[j];
                    }
                    int b = 0;
                    for(int j = 0; j < 3; j++){
                        for(int k = 0; k < 5; k++){
                            if(a[j] - 'a' == c[k]){
                                b++;
                            }
                        }
                    }
                    if(b == 0 || b == 3){
                        check = false;
                        break;
                    }
                }
            }
        }
    
        if(check == true){
            cout << '<' << s << '>' << " is acceptable." << '\n';
        }else{
            cout << '<' << s << '>' << " is not acceptable." << '\n';
        }
    }

    return 0;
}