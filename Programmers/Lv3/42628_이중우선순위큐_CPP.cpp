#include <string>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

vector<int> solution(vector<string> operations) {
    vector<int> answer;
    
    priority_queue<int> maxheap;
    priority_queue<int, vector<int>, greater<int>> minheap;
    unordered_map<int, int> cnt;
    
    for(int i = 0; i < operations.size(); i++){
        char op = ' ';
        int num = 0;
        stringstream ss(operations[i]);
        ss >> op >> num;
        
        if(op == 'I'){
            cnt[num]++;
            minheap.push(num);
            maxheap.push(num);
        }else if(op == 'D'){
            if(num == 1){
                while(!maxheap.empty() && cnt[maxheap.top()] == 0)
                    maxheap.pop();
                
                if(!maxheap.empty()){
                    cnt[maxheap.top()]--;
                    maxheap.pop();
                }
            }else{
                while(!minheap.empty() && cnt[minheap.top()] == 0)
                    minheap.pop();
                
                if(!minheap.empty()){
                    cnt[minheap.top()]--;
                    minheap.pop();
                }
            }
        }
    }
    
    while(!maxheap.empty() && cnt[maxheap.top()] == 0)
        maxheap.pop();
    while(!minheap.empty() && cnt[minheap.top()] == 0)
        minheap.pop();
    
    if(!maxheap.empty()){
        answer.push_back(maxheap.top());
        answer.push_back(minheap.top());
    }else{
        answer.push_back(0);
        answer.push_back(0);
    }
    
    return answer;
}