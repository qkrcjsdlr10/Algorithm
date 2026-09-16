#include <bits/stdc++.h>
using namespace std;

int arr1[20];
int arr2[20];

int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    int sum = 0;
    int max = 0;
    int a = 0;
    int n,m=0;

    int testCase = 0;

    cin >> testCase;

    

    for(int x = 1; x <= testCase; x++){
        cin >> n >> m;

        for(int i = 0; i < n; i++){
            cin >> arr1[i];
        }

        for(int i = 0; i < m; i++){
            cin >> arr2[i];
        }

        if(n<m){
            for(int i = 0; i <= m-n; i++){
                for(int j = 0; j < n; j++){
                    a += arr1[j] * arr2[j+i];
                    sum += a;
                    a=0;
                }
                if(sum > max){
                    max = sum;
                    sum = 0;
                }else{
                    sum = 0;
                }
            }
        }else{
            for(int i = 0; i <= n-m; i++){
                for(int j = 0; j < m; j++){
                    a += arr1[j+i] * arr2[j];
                    sum += a;
                    a=0;
                }
                if(sum > max){
                    max = sum;
                    sum = 0;
                }else{
                    sum = 0;
                }
            }
        }
        cout <<"#"<<x<<" "<< max <<"\n";
        max = 0;
        sum = 0;
        a=0;
    }

    return 0;
}