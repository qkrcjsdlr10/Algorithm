#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

int toMinutes(const string& time) {
    int hour = stoi(time.substr(0, 2));
    int minute = stoi(time.substr(3, 2));
    return hour * 60 + minute;
}

int calculateFee(int totalTime, const vector<int>& fees) {
    int basicTime = fees[0];
    int basicFee = fees[1];
    int unitTime = fees[2];
    int unitFee = fees[3];

    if (totalTime <= basicTime) {
        return basicFee;
    }

    int extraTime = totalTime - basicTime;
    int extraFee = ((extraTime + unitTime - 1) / unitTime) * unitFee;

    return basicFee + extraFee;
}

vector<int> solution(vector<int> fees, vector<string> records) {
    map<string, int> inTime;
    map<string, int> totalTime;

    for (const string& record : records) {
        string time = record.substr(0, 5);
        string carNumber = record.substr(6, 4);
        string status = record.substr(11);

        int minutes = toMinutes(time);

        if (status == "IN") {
            inTime[carNumber] = minutes;
        } else {
            totalTime[carNumber] += minutes - inTime[carNumber];
            inTime.erase(carNumber);
        }
    }

    int endOfDay = toMinutes("23:59");

    for (const auto& entry : inTime) {
        string carNumber = entry.first;
        int enterTime = entry.second;

        totalTime[carNumber] += endOfDay - enterTime;
    }

    vector<int> answer;

    for (const auto& entry : totalTime) {
        int time = entry.second;
        answer.push_back(calculateFee(time, fees));
    }

    return answer;
}