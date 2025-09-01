#include <iostream>
using namespace std;

class Solution {
    // XOR from 0..n in O(1)
    // For n < 0, define sumXor(n) = 0.
    int sumXor(long long n) {
        if (n < 0) return 0;
        switch (n % 4) {
            case 0: return (int)n;
            case 1: return 1;
            case 2: return (int)(n+1);
            default: return 0;
        }
    }
    public:
        int xorOperation(int n, int start) {
            int s = start / 2;
            int e = (n & start & 1);
            int ret = sumXor((long long)s - 1) ^ sumXor((long long)s + n -1);
            return (ret << 1) | e;
        }
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, start;
    if (!(cin >> n >> start)) {
        cerr << "Usage: provide two integers: <n> <start>\n";
        return 1;
    }

    Solution sol;
    cout << sol.xorOperation(n, start) << "\n";
    return 0;
}