#include <bits/stdc++.h>
using namespace std;

int arr[8];
int mm[12] = {1,2,1,0,1,0,1,1,0,1,0,1};
int y, m, d, t;
bool a;
string yres, mres, dres, day;


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> t;

    for(int i = 1; i <= t; i++){
        cin >> day;

        a = false;

        for(int i = 0; i < day.size(); i++){
            arr[i] = day[i] - '0';
        }

        y = arr[0] * 1000 + arr[1] * 100 + arr[2] * 10 + arr[3];
        m = arr[4] * 10 + arr[5];
        //cout << m;
        d = arr[6] * 10 + arr[7];

        if(y >= 0 && m > 0 && d > 0 && m < 13 && d < 32){
            a = true;
        }else{
            a = false;
            cout <<'#' << i << ' ' << "-1" << '\n';
            continue;
        }

        if(mm[m-1] == 1 && d < 32){
            a = true;
        }else if(mm[m-1] == 0 && d < 31){
            a = true;
        }else if(mm[m-1] == 2 && d < 29){
            a = true;
        }else{
            a = false;
        }

        if(m < 10){
            mres = '0' + to_string(m);
        }else{
            mres = to_string(m);
        }

        if(d < 10){
            dres = '0' + to_string(d);
        }else{
            dres = to_string(d);
        }

        if(y < 10){
            yres = "000" + to_string(y);
        }else if(y < 100){
            yres = "00" + to_string(y);
        }else if(y < 1000){
            yres = "0" + to_string(y);
        }else{
            yres = to_string(y);
        }

        if(a == true){
            cout << '#' << i << ' ' << yres << '/' << mres << '/' << dres << '\n';
        }else{
            cout <<'#' << i << ' ' << "-1" << '\n';
        }
        y = 0;
        d = 0;
        m = 0;
        yres = "";
        dres = "";
        mres = "";
    }

    return 0;
}