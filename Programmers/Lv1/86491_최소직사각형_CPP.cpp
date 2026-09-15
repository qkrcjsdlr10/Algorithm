#include <string>
#include <vector>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

int solution(vector<vector<int>> sizes) {
    int answer = 0;
    int hight = 0;
    int width = 0;
    
    cout << sizes.size();
    
    for(int i = 0; i < sizes.size(); i++){
        for(int j = 0; j < 2; j++){
            if(sizes[i][0] < sizes[i][1]){
                int temp = sizes[i][0];
                sizes[i][0] = sizes[i][1];
                sizes[i][1] = temp;
            }
            
            width = max(width, sizes[i][0]);
            hight = max(hight, sizes[i][1]);
        }
    }
    
    answer = width * hight;
    
    return answer;
}