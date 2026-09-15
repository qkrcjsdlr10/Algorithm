#include <string>
#include <vector>
using namespace std;

int solution(vector<string> babbling) {
    vector<string> words = {"aya", "ye", "woo", "ma"};
    int answer = 0;
    
    for (const string& word : babbling) {
        int pos = 0;
        int lastWordIdx = -1;
        bool canSpeak = true;
        
        while (pos < word.length() && canSpeak) {
            bool found = false;
            
            for (int i = 0; i < words.size(); i++) {
                // 연속된 같은 발음 방지
                if (i == lastWordIdx) continue;
                
                // 현재 위치에서 해당 발음과 일치하는지 확인
                bool match = true;
                if (pos + words[i].length() > word.length()) {
                    match = false;
                } else {
                    for (int j = 0; j < words[i].length(); j++) {
                        if (word[pos + j] != words[i][j]) {
                            match = false;
                            break;
                        }
                    }
                }
                
                if (match) {
                    pos += words[i].length();
                    lastWordIdx = i;
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                canSpeak = false;
            }
        }
        
        if (canSpeak && pos == word.length()) {
            answer++;
        }
    }
    
    return answer;
}
