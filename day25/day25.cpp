#define title "--- Day 25: Full of Hot Air ---"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<int64_t> p5;

static auto s2n(string str) {
    uint64_t n = 0;
    reverse(str.begin(), str.end());
    for (int i = 0; i < str.size(); ++i) {
        if (isdigit(str[i]))
            n += p5[i] * (str[i] - '0');
        else if (str[i] == '-')
            n -= p5[i];
        else /*if (str[i] == '=')*/
            n -= 2 * p5[i];
    }
    return n;
}

static auto n2s(uint64_t num) {
    string s;
    while (num) {
        switch (num % 5) {
        case 0:
            s += '0';
            num /= 5;
            break;
        case 1:
            s += '1';
            num /= 5;
            break;
        case 2:
            s += '2';
            num /= 5;
            break;
        case 3:
            s += '=';
            num /= 5;
            ++num;
            break;
        case 4:
            s += '-';
            num /= 5;
            ++num;
            break;
        }
    }
    reverse(s.begin(), s.end());
    return s;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day25.txt");
    p5.push_back(1);
    for (int i = 1; i <= 21; ++i) p5.push_back(p5[i - 1] * 5);
    string line;
    uint64_t sum = 0;
    while (getline(f, line)) sum += s2n(line);
    cout << title << endl
         << "Part 1  - " << n2s(sum) << endl
         << "Part 2  - free star" << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
