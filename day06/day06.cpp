#define title "--- Day 6: Tuning Trouble  ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;

static auto all_diff(char* c, int l) {
    for (int i = 0; i < l - 1; ++i)
        for (int j = i + 1; j < l; ++j)
            if (c[i] == c[j]) return false;
    return true;
}

static void rotate(char* cn, char c, int l) {
    for (int i = 0; i < l - 1; ++i) cn[i] = cn[i + 1];
    cn[l - 1] = c;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day06.txt");
    char c;
    char c4[4] = {0};
    int n1 = 1;
    while (f >> c) {
        rotate(c4, c, 4);
        if ((n1 > 4) && all_diff(c4, 4)) break;
        ++n1;
    }
    f.clear();
    f.seekg(0, ios_base::beg);
    char c14[14] = {0};
    int n2 = 1;
    while (f >> c) {
        rotate(c14, c, 14);
        if ((n2 > 14) && all_diff(c14, 14)) break;
        ++n2;
    }
    cout << title << endl
         << "Part 1 - " << n1 << endl
         << "Part 2 - " << n2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
