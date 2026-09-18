#include <bits/stdc++.h>
using namespace std;

int n,m;
int arr[100][100];
int visit[100][100];
int dx[4] = {0, 1, 0, -1};
int dy[4] = {-1, 0, 1, 0};
int detect;


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m;

    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            cin >> arr[i][j];
        }
    }

    for(int tmp = 0; tmp < 64; tmp++){
        int brute = tmp;
        for(int i = 0; i < 3; i++){
            cout << brute%4;
            brute /= 4;
        }

        cout << '\n';
    }

    return 0;
}