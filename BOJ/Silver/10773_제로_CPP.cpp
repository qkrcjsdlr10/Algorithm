#include <bits/stdc++.h>
using namespace std;

// int arr[100000];
int n;

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int sum = 0;
    int cnt = 0;
    int idx = 0;
    int a;
    stack<int> s;

    cin >> n;

    for(int i = 0; i < n; i++){
        cin >> a;
        s.push(a);
    }

    // while(!s.empty()){
    //     if(s.top() != 0 && cnt == 0){
    //         sum += s.top();
    //         s.pop();
    //     }else if(s.top() == 0){
    //         cnt++;
    //         s.pop();
    //     }else{
    //         s.pop();
    //         cnt--;
    //     }
    // }

    

    // for(int i = 0; i < n; i++){
    //     sum += arr[i];
    //     // cout << "arr[i]= " << arr[i] << "sum= " << sum << '\n';
    //     if(i > 0 && arr[i] == 0){
    //         for(int j = i; j >= 0; j--){
    //             if(arr[j] != 0){
    //                 // cout << "arr[j]= " << arr[j] << '\n';
    //                 sum -= arr[j];
    //                 arr[j] = 0;
    //                 // cnt++;
    //                 break;
    //             }
    //         }
    //     }
    //     // cout << "arr[i]= " << arr[i] << "sum= " << sum << '\n';
        
    // }

    // int lastNonZero[100000] = {};

    int* arr = new int[n];       // 입력 배열
    int* lastNonZero = new int[n]; // 최근 값을 저장하는 스택 역할 배열

    for (int i = 0; i < n; i++) {
        if (arr[i] != 0) {
            // 값이 0이 아닌 경우: 스택에 추가하고 합산에 포함
            lastNonZero[idx] = arr[i];
            idx++;
            sum += arr[i];
        } else if (arr[i] == 0 && idx > 0) {
            // 값이 0인 경우: 가장 최근 값을 제거하고 합산에서 차감
            idx--; // 스택 포인터 감소
            sum -= lastNonZero[idx];
        }
    }

    cout << sum;

    return 0;
}