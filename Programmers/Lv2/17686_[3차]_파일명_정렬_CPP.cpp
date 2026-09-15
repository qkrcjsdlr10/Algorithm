#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class File{
    public:
    string origin, head, tail;
    int number, idx;
    
    File(string o, string h, int n, int i)
        :origin(o), head(h), number(n), idx(i) {}
};

string toLowerStr(string s){
    for(auto &c : s){
        c = tolower((unsigned char)c);
    }
    return s;
}

File parseFile(string file, int idx){
    int n = file.size();
    int i = 0;
    
    while(i < n && !isdigit((unsigned char)file[i])) i++;
    string head = toLowerStr(file.substr(0, i));
    
    int j = i;
    while(j < n && isdigit((unsigned char)file[j])) j++;
    int num = stoi(file.substr(i, j-i));
    
    return File(file, head, num, idx);
}

bool cmp(File& a, File& b){
    if(a.head != b.head) return a.head < b.head;
    if(a.number != b.number) return a.number < b.number;
    return a.idx < b.idx;
}

vector<string> solution(vector<string> files) {
    vector<string> answer;
    vector<File> v;
    
    for(int i = 0; i < files.size(); i++){
        v.push_back(parseFile(files[i], i));
    }
    
    sort(v.begin(), v.end(), cmp);
    
    for(auto &f : v){
        answer.push_back(f.origin);
    }
    
    
    return answer;
}