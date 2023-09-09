#define title "--- Day 10: Cathode-Ray Tube ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace chrono;

static char screen[240] = {0};

static auto check_cycle(int cycle, int x) {
    int scr_x = (cycle - 1) % 40;
    if (abs(scr_x - x) < 2) screen[cycle - 1] = 1;
    if ((cycle - 20) % 40) return 0;
    return cycle * x;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day10.txt");
    string opcode;
    int cycle = 0, x = 1, strength = 0;
    while (f >> opcode) {
        strength += check_cycle(++cycle, x);
        if (opcode == "addx") {
            strength += check_cycle(++cycle, x);
            int operand;
            f >> operand;
            x += operand;
        }
    }
    cout << title << endl << "Part 1  - " << strength << endl << "Part 2  - " << endl;
    for (int v = 0; v < 6; ++v) {
        for (int h = 0; h < 40; ++h) cout << (screen[40 * v + h] ? '#' : ' ');
        cout << endl;
    }
    cout << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
