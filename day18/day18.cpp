#define title "--- Day 18: Boiling Boulders ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

using namespace std;
using namespace chrono;

struct point {
    int8_t x, y, z;
    auto inside() const {
        return ((x >= -1) && (x <= 20) && (y >= -1) && (y <= 20) && (z >= -1) && (z <= 20));
    }
    auto operator+(const point& r) const {
        return point({int8_t(x + r.x), int8_t(y + r.y), int8_t(z + r.z)});
    }
    auto operator<(const point& r) const {
        if (z == r.z) {
            if (y == r.y) return x < r.x;
            return y < r.y;
        }
        return z < r.z;
    }
};

static const point neighbor[] = {{1, 0, 0},  {-1, 0, 0}, {0, 1, 0},
                                 {0, -1, 0}, {0, 0, 1},  {0, 0, -1}};

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day18.txt");
    string line;
    set<point> cubes;
    while (getline(f, line)) {
        int x, y, z;
        (void)sscanf(line.c_str(), "%d,%d,%d", &x, &y, &z);
        cubes.insert({int8_t(x), int8_t(y), int8_t(z)});
    }
    set<point> water, neighbors;
    neighbors.insert({0, 0, 0});
    while (!neighbors.empty()) {
        set<point> next;
        for (const auto& a : neighbors)
            if (a.inside() && !water.count(a) && !cubes.count(a)) {
                water.insert(a);
                for (const auto& d : neighbor) next.insert(a + d);
            }
        neighbors = next;
    }
    int part1 = 0, part2 = 0;
    for (const auto& c : cubes) {
        for (const auto& d : neighbor) {
            if (!cubes.count(c + d)) ++part1;
            if (water.count(c + d)) ++part2;
        }
    }
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
