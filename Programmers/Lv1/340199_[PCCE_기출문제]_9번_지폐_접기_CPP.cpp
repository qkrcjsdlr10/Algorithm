#include <string>
#include <vector>
#include <iostream>

using namespace std;
int res = 0;

void func(int w, int ww, int h, int wh){
    if(w < h){
        int temp = w;
        w = h;
        h = temp;
    }
    if(w <= ww && h <= wh){
        // cout << w << ' ' << h << ' ';
        return;
    }
    cout << w << ' ' << h << ' ' << res << ' ';
    
    
    res++;
    func(w/2, ww, h, wh);
}

int solution(vector<int> wallet, vector<int> bill) {
    int answer = 0;
    
    int width = bill[0];
    int hight = bill[1];
    int w_width = wallet[0];
    int w_hight = wallet[1];
    
    if(bill[0] < bill[1]){
        width = bill[1];
        hight = bill[0];
    }
    
    if(wallet[0] < wallet[1]){
        w_width = wallet[1];
        w_hight = wallet[0];
    }
    
    func(width, w_width, hight, w_hight);
    
    // cout << width << ' ' << hight << '\n' << bill[0] << ' ' << bill[1];
    
//     while(1){
//         if(w_width >= width && w_hight >= hight){
//             break;
//         }
//         if(w_width < width){
//             width /= 2;
//             cout << width << ' ';
//         }
        
//         if(width < hight){
//             int temp = width;
//             width = hight;
//             hight = temp;
//         }
//         answer ++;
//     }
    answer = res;
    return answer;
}