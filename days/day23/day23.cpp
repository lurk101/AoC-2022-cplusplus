#define title "--- Day 23: Unstable Diffusion ---"
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>

using namespace std;
using namespace chrono;

const int NORTH = 0;
const int SOUTH = 1;
const int WEST = 2;
const int EAST = 3;

struct point {
    int x, y;
    bool operator!=(const point& r) const { return x != r.x || y != r.y; }
    bool operator<(const point& r) const {
        if (y == r.y)
            return x < r.x;
        return y < r.y;
    }
};

set<point> elves;

point propose(int dir_ix, const point& elf) {
    bool should_move = false;
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == dy && dy == 0)
                continue;
            if (elves.count({elf.x + dx, elf.y + dy})) {
                should_move = true;
                break;
            }
        }
    if (!should_move)
        return point({elf.x, elf.y});
    for (int i = 0; i < 4; i++) {
        switch ((dir_ix + i) % 4) {
        case NORTH:
            if (!elves.count({elf.x + 1, elf.y - 1}) && !elves.count({elf.x - 1, elf.y - 1}) &&
                !elves.count({elf.x, elf.y - 1})) {
                return point({elf.x, elf.y - 1});
            } else
                break;
        case SOUTH:
            if (!elves.count({elf.x + 1, elf.y + 1}) && !elves.count({elf.x - 1, elf.y + 1}) &&
                !elves.count({elf.x, elf.y + 1})) {
                return point({elf.x, elf.y + 1});
            } else
                break;
        case WEST:
            if (!elves.count({elf.x - 1, elf.y - 1}) && !elves.count({elf.x - 1, elf.y + 1}) &&
                !elves.count({elf.x - 1, elf.y})) {
                return point({elf.x - 1, elf.y});
            } else
                break;
        case EAST:
            if (!elves.count({elf.x + 1, elf.y - 1}) && !elves.count({elf.x + 1, elf.y + 1}) &&
                !elves.count({elf.x + 1, elf.y})) {
                return point({elf.x + 1, elf.y});
            } else
                break;
        }
    }
    return {elf.x, elf.y};
};

bool mov(int dir_ix) {
    map<point, point> proposed;
    map<point, int> to_counts;
    for (auto& elf : elves) {
        auto proposed_move = propose(dir_ix, elf);
        proposed[elf] = proposed_move;
        if (to_counts.count(proposed_move))
            to_counts[proposed_move]++;
        else
            to_counts[proposed_move] = 1;
    }
    set<point> new_elves;
    bool elves_moved = false;
    for (auto& e : elves) {
        auto& proposed_move = proposed[e];
        if (to_counts[proposed_move] == 1) {
            new_elves.insert(proposed_move);
            if (proposed_move != e)
                elves_moved = true;
        } else
            new_elves.insert(e);
    }
    elves = new_elves;
    return elves_moved;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream input("day23.txt");
    string line;
    int y = 0;
    while (getline(input, line)) {
        int x = 0;
        for (auto& c : line) {
            if (c == '#')
                elves.insert({x, y});
            x++;
        }
        y++;
    }
    int dir_ix = 0;
    for (int i = 0; i < 10; i++) {
        mov(dir_ix);
        dir_ix = (dir_ix + 1) % 4;
    }
    int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
    for (auto& e : elves) {
        if (e.x < min_x)
            min_x = e.x;
        if (e.x > max_x)
            max_x = e.x;
        if (e.y < min_y)
            min_y = e.y;
        if (e.y > max_y)
            max_y = e.y;
    }
    int part1 = (max_y - min_y + 1) * (max_x - min_x + 1) - int(elves.size());
    int i;
    for (i = 11;; i++) {
        if (!mov(dir_ix))
            break;
        dir_ix = (dir_ix + 1) % 4;
    }
    cout << "Part 2: " << i + 1 << endl;
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << i << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
