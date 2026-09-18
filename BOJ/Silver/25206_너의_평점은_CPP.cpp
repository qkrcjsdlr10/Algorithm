#include<bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);
    double sum = 0;   
    int time = 0;

    int arr[10] = {};

    for(int i = 0; i < 20; i++){
        string s = "";
        string str = "";

        double f = 0;
        double d = 0;

        getline(cin, s);
        int a = 0;
        for(auto c : s){
            if(c != ' '){
                str += c;
            }
            
            if(c == ' ' || a == s.size() -1){
                if(str[0] >= '0' && str[0] <= '9'){
                    f = stod(str);
                }

                if(str != "P"){
                    if(str == "A+"){
                        d = 4.5;
                    }else if(str == "A0"){
                        d = 4;
                    }else if(str == "B+"){
                        d = 3.5;
                    }else if(str == "B0"){
                        d = 3;
                    }else if(str == "C+"){
                        d = 2.5;
                    }else if(str == "C0"){
                        d = 2;
                    }else if(str == "D+"){
                        d = 1.5;
                    }else if(str == "D0"){
                        d = 1;
                    }else if(str == "F"){
                        d = 0;
                    }
                    
                }else{
                    f = 0;
                    d = 0;
                    str = "";
                }
                str = "";
            }
            a++;
        }

        sum += d * f;
        time += f;
        a = 0;
        f = 0;
        d = 0;

    }
    cout << fixed << setprecision(6) << sum / time;

    return 0;
}