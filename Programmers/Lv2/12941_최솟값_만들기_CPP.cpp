#include <iostream>
#include<vector>
#include<algorithm>
using namespace std;

int solution(vector<int> A, vector<int> B)
{
    int answer = 0;
    vector<bool> check;
    check.resize(A.size());
    sort(A.begin(), A.end());
    sort(B.rbegin(), B.rend());

    for(int i = 0; i < A.size(); i++){
        answer += A[i] * B[i];
        // cout << A[i] * B[i] << ' ';
        // cout << answer << ' ';
    }

    return answer;
}