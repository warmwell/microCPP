#include<iostream>
#include<vector>
#include<algorithm>
#include<sstream>
using namespace std;

// 给你一个整数数组 bloomDay，以及两个整数 m 和 k 。
// 现需要制作 m 束花。制作花束时，需要使用花园中 相邻的 k 朵花 。
// 花园中有 n 朵花，第 i 朵花会在 bloomDay[i] 时盛开，恰好 可以用于 一束 花中。
// 请你返回从花园中摘 m 束花需要等待的最少的天数。如果不能摘到 m 束花则返回-1.

class Solution {
public:
    int minDays(vector<int>& bloomDay, int m, int k) {
        if (bloomDay.size() < m * static_cast<__int64>(k)) {
            return INVALID_RET_VAL;
        }

        stringstream ss;
        for (auto day : bloomDay) {
            ss << day;
            ss << " ";
        }

        cout << "bloomDays:" << ss.str() << "m: " << m << " k: " << k << endl;
        int bloomDayMax = *max_element(bloomDay.cbegin(), bloomDay.cend());
        int bloomDayMin = *min_element(bloomDay.cbegin(), bloomDay.cend());
        int high = bloomDayMax,low = bloomDayMin;
        cout << "max: " << bloomDayMax << " min: " << bloomDayMin << endl;

        while(low < high) {
            int middle = (low + high) / 2;
            cout << "middle vale: " << middle << endl;
            if (CheckNdaysIsOk(bloomDay, middle, m, k)) {
                high = middle;
            } else {
                low = middle + 1;
            }
        }

        return low;
    }
private:
    bool CheckNdaysIsOk(const vector<int> &bloomDay, int n, int m, int k)
    {
        int curBloomM_Num = 0;
        int curBloomK_Num = 0;
        // 输入：bloomDay = [1,10,2,9,3,8,4,7,5,6], m = 4, k = 2
        // 输出：9
        for (int i = 0; i < bloomDay.size() && curBloomM_Num < m; i++) {
            curBloomK_Num++;
            if (bloomDay[i] > n) {
                curBloomK_Num = 0;
            } else if (curBloomK_Num == k) {
                curBloomM_Num++;
                curBloomK_Num = 0;
            }
        }

        return curBloomM_Num >= m;
    }
    const int INVALID_RET_VAL = -1;
};

int main()
{
    vector<int> bloomDays = {1,10,2,9,3,8,4,7,5,6};
    int m = 4, k = 2;
    cout << Solution().minDays(bloomDays, m, k) << endl;
    return 0;
}