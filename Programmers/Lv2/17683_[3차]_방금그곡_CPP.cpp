#include <string>
#include <vector>
#include <sstream>
using namespace std;

string convert(string s) {
    string res = "";
    
    for (int i = 0; i < s.size(); i++) {
        if (i + 1 < s.size() && s[i + 1] == '#') {
            if (s[i] == 'C') res += 'c';
            else if (s[i] == 'D') res += 'd';
            else if (s[i] == 'F') res += 'f';
            else if (s[i] == 'G') res += 'g';
            else if (s[i] == 'A') res += 'a';
            i++;
        } else {
            res += s[i];
        }
    }
    
    return res;
}

int getTime(string start, string end) {
    int sh = stoi(start.substr(0, 2));
    int sm = stoi(start.substr(3, 2));
    int eh = stoi(end.substr(0, 2));
    int em = stoi(end.substr(3, 2));
    
    return (eh * 60 + em) - (sh * 60 + sm);
}

string solution(string m, vector<string> musicinfos) {
    string answer = "(None)";
    int maxTime = -1;
    
    m = convert(m);
    
    for (string info : musicinfos) {
        string start, end, name, melody;
        stringstream ss(info);
        
        getline(ss, start, ',');
        getline(ss, end, ',');
        getline(ss, name, ',');
        getline(ss, melody, ',');
        
        int playTime = getTime(start, end);
        melody = convert(melody);
        
        string played = "";
        for (int i = 0; i < playTime; i++) {
            played += melody[i % melody.size()];
        }
        
        if (played.find(m) != string::npos) {
            if (playTime > maxTime) {
                maxTime = playTime;
                answer = name;
            }
        }
    }
    
    return answer;
}