#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// 노래 정렬: 재생수 ↓, 인덱스 ↑
bool cmpSong(pair<int,int> a, pair<int,int> b) {
    if (a.first == b.first) return a.second < b.second;
    return a.first > b.first;
}

// 장르 정렬: 총 재생수 ↓
bool cmpGenre(pair<string,int> a, pair<string,int> b) {
    return a.second > b.second;
}

vector<int> solution(vector<string> genres, vector<int> plays) {
    vector<int> answer;

    // 1. 장르별 노래 저장
    unordered_map<string, vector<pair<int,int>>> mp;
    // 2. 장르별 총 재생수
    unordered_map<string, int> total;

    for (int i = 0; i < genres.size(); i++) {
        mp[genres[i]].push_back({plays[i], i});
        total[genres[i]] += plays[i];
    }

    // 3. 장르 정렬용 vector
    vector<pair<string,int>> v;
    for (auto& p : total) {
        v.push_back({p.first, p.second});
    }

    sort(v.begin(), v.end(), cmpGenre);

    // 4. 장르 순서대로 처리
    for (auto& g : v) {
        string genre = g.first;

        // 노래 정렬
        sort(mp[genre].begin(), mp[genre].end(), cmpSong);

        // 최대 2개 선택
        int cnt = 0;
        for (auto& song : mp[genre]) {
            answer.push_back(song.second);
            cnt++;
            if (cnt == 2) break;
        }
    }

    return answer;
}