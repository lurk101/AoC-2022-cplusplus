#define title "--- Day 3: Rucksack Reorganization ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;

static int toprio(char c) { return c > 'Z' ? c - 'a' + 1 : c - 'A' + 27; }

static auto part1(string& line) {
    int n = line.size(), h = n / 2;
    for (int i = 0; i < h; ++i)
        for (int j = h; j < n; ++j)
            if (line[i] == line[j]) return toprio(line[i]);
    return 0;
}

static string l1, l2, l3;

static auto part2(string& l1, string& l2, string& l3) {
    char count1[53] = {0}, count2[53] = {0}, count3[53] = {0};
    for (auto c : l1) count1[toprio(c)] = 1;
    for (auto c : l2) count2[toprio(c)] = 1;
    for (auto c : l3) count3[toprio(c)] = 1;
    for (int i = 1; i < 53; ++i)
        if (count1[i] && count2[i] && count3[i]) return i;
    return 0;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day03.txt");
    int prio = 0, badges = 0;
    while (getline(f, l1)) {
        getline(f, l2);
        getline(f, l3);
        prio += part1(l1) + part1(l2) + part1(l3);
        badges += part2(l1, l2, l3);
    }
    cout << title << endl
         << "Part 1 - " << prio << endl
         << "Part 2 - " << badges << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
