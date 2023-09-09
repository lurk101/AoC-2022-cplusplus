#define title "--- Day 4: Camp Cleanup ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;

static auto contains(int l1, int h1, int l2, int h2) {
    if (l1 < l2) return h1 >= h2;
    if (l2 < l1) return h2 >= h1;
    return true;
}

static auto overlaps(int l1, int h1, int l2, int h2) {
    if (l1 < l2) return h1 >= l2;
    if (l2 < l1) return h2 >= l1;
    return true;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day04.txt");
    string l;
    int contained = 0, overlapped = 0;
    while (getline(f, l)) {
        int l1, h1, l2, h2;
        (void)sscanf(l.c_str(), "%d-%d,%d-%d", &l1, &h1, &l2, &h2);
        if (contains(l1, h1, l2, h2)) ++contained;
        if (overlaps(l1, h1, l2, h2)) ++overlapped;
    }
    cout << title << endl
         << "Part 1 - " << contained << endl
         << "Part 2 - " << overlapped << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
