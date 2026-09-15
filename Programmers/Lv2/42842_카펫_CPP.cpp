#include <string>
#include <vector>
#include <iostream>
using namespace std;

vector<int> solution(int brown, int yellow) {
    vector<int> answer;
    
    vector<vector<int>> arr;
    
    int mx = brown + yellow;
    
    
    int hight = 3;
    
    int width = mx / hight;
    
    int a = hight - 2;
    int b = width - 2;
    
    int cur = a * b;
    cout << width << ' ' <<  hight << ' ' << cur << '\n';
    while(cur != yellow){
        hight++;
        width = mx / hight;
        
        a = hight - 2;
        b = width - 2;
        cur = a * b;
        
        // cout << width << ' ' <<  hight << ' ' << cur << '\n';
    }
    
    
    // arr.resize(width, vector<int> (hight));
    
    // cout << hight << ' ' << width;
    answer.push_back(width);
    answer.push_back(hight);
    return answer;
}