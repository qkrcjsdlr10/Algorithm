#include<bits/stdc++.h>
using namespace std;

vector<int> arr;
//1 2 3 4 5 
void deletearr(int pos, int cnt){
    for(int i = pos; i < arr.size() - cnt; i++){
        arr[i] = arr[i + cnt];
    }
    arr.resize(arr.size() - cnt);
}

void insertarr(int pos, int cnt){
    arr.resize(arr.size() + cnt);

    for(int i = arr.size() - cnt - 1; i >= pos; i--){
        arr[i + cnt] = arr[i];
    }
}

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);


    for(int t = 1; t <= 10; t++){
        int n = 0;
        cin >> n;
        
        while(n--){
            int num = 0;
            cin >> num;
            arr.push_back(num);
        }
        int cmdcnt = 0;
        cin >> cmdcnt;

        // deletearr(1, 1);

        
        while(cmdcnt--){
            char cmd = ' ';
            cin >> cmd;
            int cur = 0;
            int cnt = 0;
            if(cmd == 'I'){
                cin >> cur >> cnt;
                insertarr(cur, cnt);
                while(cnt--){
                    int insertnum = 0;
                    cin >> insertnum;
                    arr[cur] = insertnum;
                    cur++;
                }
            }else if(cmd == 'D'){
                cin >> cur >> cnt;
                deletearr(cur, cnt);
            }else if(cmd == 'A'){
                cin >> cnt;
                while(cnt--){
                    int insertnum = 0;
                    cin >> insertnum;
                    arr.push_back(insertnum);
                }
            }
        }

        cout << '#' << t << ' ';
        for(int i = 0; i < 10; i++){
            cout << arr[i] << ' ';
        }
        cout << '\n';
        arr.clear();
    }
    
    return 0;
}