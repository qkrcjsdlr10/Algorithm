#include <string>
#include <vector>

using namespace std;

string solution(string my_string, string letter) {
    int i = 0; // 현재 처리 중인 인덱스
    
    while (i < my_string.size()) {
        if (my_string[i] == letter[0]) {
            // 일치하는 문자를 제거하고, 남은 문자들을 당겨옴
            for (int j = i; j < my_string.size() - 1; j++) {
                my_string[j] = my_string[j + 1];
            }
            my_string.resize(my_string.size() - 1); // 문자열 크기 조정
        } else {
            i++; // 일치하지 않으면 다음 인덱스로 이동
        }
    }
    
    return my_string;
}
