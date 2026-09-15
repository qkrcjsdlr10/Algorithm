#include <string>
#include <vector>
#include <set>

using namespace std;

int solution(vector<int> elements) {
    int answer = 0;
    int size = 1;
    int n = elements.size();
    set<int> st;
    
    for(int len = 1; len <= n; len++){
        for(int start = 0; start < n; start++){
            int sum = 0;
            for(int k = 0; k < len; k++){
                sum += elements[(start + k) % n];
            }
            st.insert(sum);
        }
    }
    answer = st.size();
    return answer;
}