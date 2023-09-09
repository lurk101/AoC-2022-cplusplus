#define title "--- Day 23: Unstable Diffusion ---"
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;
using namespace chrono;

static const int NORTH = 0;
static const int SOUTH = 1;
static const int WEST = 2;
static const int EAST = 3;

struct point {
    int x, y;
    auto operator==(const point& r) const { return (x == r.x) && (y == r.y); }
    auto operator+(const point& r) const { return point({x + r.x, y + r.y}); }
};

struct hash_fn {
    auto operator()(const point& p) const { return (p.x + p.y) * (p.x + p.y + 1) / 2 + p.x; }
};

static const point N({0, -1}), S({0, 1}), E({1, 0}), W({-1, 0}), NE(N + E), NW(N + W), SE(S + E),
    SW(S + W);

static unordered_set<point, hash_fn> elves;
static int dir_ix;

static auto propose(const point& elf) {
    bool should_move = false;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if ((dx == 0) && (dy == 0)) continue;
            if (elves.count(elf + point({dx, dy}))) {
                should_move = true;
                break;
            }
        }
    }
    if (!should_move) return elf;
    for (int i = 0; i < 4; i++) {
        switch ((dir_ix + i) % 4) {
        case NORTH:
            if (!elves.count(elf + N) && !elves.count(elf + NE) && !elves.count(elf + NW))
                return point(elf + N);
            break;
        case SOUTH:
            if (!elves.count(elf + S) && !elves.count(elf + SE) && !elves.count(elf + SW))
                return point(elf + S);
            break;
        case WEST:
            if (!elves.count(elf + W) && !elves.count(elf + NW) && !elves.count(elf + SW))
                return point(elf + W);
            break;
        case EAST:
            if (!elves.count(elf + E) && !elves.count(elf + NE) && !elves.count(elf + SE))
                return point(elf + E);
            break;
        }
    }
    return elf;
};

static auto mov() {
    unordered_map<point, point, hash_fn> proposed;
    unordered_map<point, int, hash_fn> to_counts;
    for (auto& elf : elves) {
        auto proposed_move = propose(elf);
        proposed[elf] = proposed_move;
        if (to_counts.count(proposed_move))
            to_counts[proposed_move]++;
        else
            to_counts[proposed_move] = 1;
    }
    unordered_set<point, hash_fn> new_elves;
    bool moved = false;
    for (auto& e : elves) {
        auto& proposed_move = proposed[e];
        if (to_counts[proposed_move] == 1) {
            new_elves.insert(proposed_move);
            if (!(proposed_move == e)) moved = true;
        } else
            new_elves.insert(e);
    }
    elves = new_elves;
    dir_ix = (dir_ix + 1) % 4;
    return moved;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream input("day23.txt");
    string line;
    int y = 0;
    while (getline(input, line)) {
        int x = 0;
        for (auto& c : line) {
            if (c == '#') elves.insert({x, y});
            x++;
        }
        y++;
    }
    int i;
    for (i = 1; i <= 10; i++) mov();
    int min_x = INT_MAX, max_x = INT_MIN, min_y = INT_MAX, max_y = INT_MIN;
    for (auto& e : elves) {
        if (e.x < min_x) min_x = e.x;
        if (e.x > max_x) max_x = e.x;
        if (e.y < min_y) min_y = e.y;
        if (e.y > max_y) max_y = e.y;
    }
    int part1((max_y - min_y + 1) * (max_x - min_x + 1) - int(elves.size()));
    for (;; i++)
        if (!mov()) break;
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << i << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
