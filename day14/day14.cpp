#define title "--- Day 14: Regolith Reservoir ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

struct point {
    int x, y;
};

static map<int, set<int>> cave;
static int max_y = 0;

static auto get_point(stringstream& ss) {
    char c;
    point p;
    ss >> p.x;
    ss >> c;
    ss >> p.y;
    if (p.y > max_y) max_y = p.y;
    return p;
}

static void add_seam(string s) {
    stringstream ss(s);
    point start = get_point(ss);
    string delim;
    ss >> delim;
    while (delim == "->") {
        point next = get_point(ss);
        if (start.x == next.x) {
            if (start.y < next.y)
                for (int y = start.y; y <= next.y; y++) cave[start.x].insert(y);
            else
                for (int y = start.y; y >= next.y; y--) cave[start.x].insert(y);
        } else {
            if (start.x < next.x)
                for (int x = start.x; x <= next.x; x++) cave[x].insert(start.y);
            else
                for (int x = start.x; x >= next.x; x--) cave[x].insert(start.y);
        }
        start = next;
        if (!(ss >> delim)) delim = "";
    }
}

static auto drop_sand(int part) {
    point sand = {500, 0};
    bool first = true;
    while (true) {
        if (part == 1) {
            bool lower = false;
            for (auto& c : cave[sand.x])
                if (c > sand.y) {
                    lower = true;
                    break;
                }
            if (!lower) return true;
        } else if (sand.y + 1 == max_y) {
            cave[sand.x].insert(sand.y);
            break;
        }
        if (!cave[sand.x].contains(sand.y + 1))
            sand.y++;
        else if (!cave[sand.x - 1].contains(sand.y + 1)) {
            sand.y++;
            sand.x--;
        } else if (!cave[sand.x + 1].contains(sand.y + 1)) {
            sand.y++;
            sand.x++;
        } else {
            if (sand.x == 500 && sand.y == 0) return true;
            cave[sand.x].insert(sand.y);
            break;
        }
    }
    return false;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day14.txt");
    string s;
    while (getline(f, s)) {
        add_seam(s);
    }
    map<int, set<int>> save_cave = cave;
    int sand;
    for (sand = 0;; sand++)
        if (drop_sand(1)) break;
    cout << title << endl << "Part 1  - " << sand << endl;
    cave = save_cave;
    max_y += 2;
    for (sand = 1;; sand++)
        if (drop_sand(2)) break;
    cout << "Part 2  - " << sand << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
