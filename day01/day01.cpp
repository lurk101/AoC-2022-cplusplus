#define title "--- Day 1: Calorie Counting ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

int main() {
    int c1 = 0, c2 = 0, c3 = 0, c = 0;
    auto start = high_resolution_clock::now();
    ifstream f("day01.txt");
    string line;
    while (getline(f, line)) {
        if (line == "") {
            if (c > c1) {
                c3 = c2;
                c2 = c1;
                c1 = c;
            } else if (c > c2) {
                c3 = c2;
                c2 = c;
            } else if (c > c3)
                c3 = c;
            c = 0;
            continue;
        }
        c += stol(line);
    }
    cout << title << endl
         << "Part 1 - " << c1 << endl
         << "Part 2 - " << c1 + c2 + c3 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
