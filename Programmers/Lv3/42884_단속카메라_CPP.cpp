#include <string>
#include <vector>
#include <algorithm>
#include <utility>

using namespace std;

bool cmp(vector<int> a, vector<int> b){
    if(a[1]<b[1]) return 1;
    return 0;
}

int solution(vector<vector<int>> routes) {
    int answer = 0;
    vector<vector<pair<int,int>>> v;
    sort(routes.begin(), routes.end(),cmp);
    for(int i=0;i<routes.size();++i){
        // printf("%d %d \n",routes[i][0],routes[i][1]);
        vector<pair<int,int>> v1;
        v1.push_back({routes[i][0],routes[i][1]});
        if(v.size()){
            int canPut=0;
            for(int j=0;j<v.size();++j){
                if(v[j][v[j].size()-1].second < routes[i][0]){
                    v[j].push_back({routes[i][0],routes[i][1]});
                    canPut=1;
                }
            }
            if(!canPut){
                v.push_back(v1);
            }
        } else {
            v.push_back(v1);
        }
    }
    for(int i=0;i<v.size();++i){
        for(int j=0;j<v[i].size();++j){
            // printf("%d %d, ",v[i][j].first, v[i][j].second);
        }
        // printf("\n");
    }
    answer = v[0].size();

    return answer;
}