#define title "--- Day 20: Grove Positioning System ---"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static int mod(int64_t a, int b) { return (a % b + b) % b; }

static void shuffle(vector<pair<int64_t, int>>& num, int n) {
    for (int i = 0; i < n; ++i) {
        int j = int(find_if(num.begin(), num.end(),
                            [=](const pair<int64_t, int>& t) { return t.second == i; }) -
                    num.begin());
        int64_t val = num[j].first;
        num.erase(num.begin() + j);
        j = mod(j + val, n - 1);
        num.insert(num.begin() + j, {val, i});
    }
}

static int find_0(vector<pair<int64_t, int>>& num, int n) {
    auto z =
        find_if(num.begin(), num.end(), [=](const pair<int64_t, int>& t) { return t.first == 0; });
    return int(z - num.begin());
}

static auto sum(vector<pair<int64_t, int>>& num, int n, int z) {
    int64_t part = 0;
    for (int t : {1000, 2000, 3000}) part += num[mod(z + t, n)].first;
    return part;
}

int main(void) {
    auto strt = high_resolution_clock::now();
    vector<pair<int64_t, int>> num1;
    vector<pair<int64_t, int>> num2;
    ifstream f("day20.txt");
    string line;
    int n = 0;
    while (getline(f, line)) {
        int val = stoi(line);
        num1.push_back({val, n});
        num2.push_back({val * 811589153LL, n});
        ++n;
    }
    shuffle(num1, n);
    for (int k = 0; k < 10; ++k) shuffle(num2, n);
    cout << title << endl
         << "Part 1  - " << sum(num1, n, find_0(num1, n)) << endl
         << "Part 2  - " << sum(num2, n, find_0(num2, n)) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
