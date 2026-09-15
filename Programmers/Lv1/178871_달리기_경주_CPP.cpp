#include <string>
#include <vector>
#include <unordered_map>
using namespace std;

vector<string> solution(vector<string> players, vector<string> callings) {
    unordered_map<string, int> pos;

    for (int i = 0; i < players.size(); i++) {
        pos[players[i]] = i;
    }

    for (int i = 0; i < callings.size(); i++) {
        int idx = pos[callings[i]];
        string frontPlayer = players[idx - 1];

        swap(players[idx], players[idx - 1]);

        pos[callings[i]]--;
        pos[frontPlayer]++;
    }

    return players;
}