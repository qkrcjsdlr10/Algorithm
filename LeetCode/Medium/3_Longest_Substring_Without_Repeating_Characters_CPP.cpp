class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        vector<int> freq(128, 0);   // 모든 ASCII 문자 처리
        int left = 0;
        int mx = 0;

        for (int right = 0; right < s.size(); right++) {
            freq[s[right]]++;

            while (freq[s[right]] > 1) {
                freq[s[left]]--;
                left++;
            }

            mx = max(mx, right - left + 1);
        }

        return mx;
    }
};