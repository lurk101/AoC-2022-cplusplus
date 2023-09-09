#define title "--- Day 17: Pyroclastic Flow ---"
#include <array>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

using namespace std;
using namespace chrono;

// clang-format off
static const set<pair<int, int>> blocks[5] = { 
     {{3, 3}, {4, 3}, {5, 3}, {6, 3}},
     {{4, 5}, {3, 4}, {4, 4}, {5, 4}, {4, 3}},
     {{3, 3}, {4, 3}, {5, 3}, {5, 4}, {5, 5}},
     {{3, 3}, {3, 4}, {3, 5}, {3, 6}},
     {{3, 3}, {4, 3}, {3, 4}, {4, 4}}
};
// clang-format on

static auto operator+(const set<pair<int, int>>& block, int dy) {
    set<pair<int, int>> new_block;
    for (const auto& p : block) {
        const auto& [x, y] = p;
        new_block.insert(pair<int, int>{x, (y + dy)});
    }
    return new_block;
}

static auto blow(set<pair<int, int>> block, bool left) {
    set<pair<int, int>> shoved;
    for (const auto& p : block) {
        const auto& [x, y] = p;
        auto dx = (left) ? -1 : 1;
        shoved.insert(pair<int, int>{(x + dx), y});
    }
    return shoved;
}

static void fall(vector<uint8_t>& silo, const set<pair<int, int>>& block) {
    for (const auto& p : block) {
        const auto& [x, y] = p;
        while (silo.size() <= y) silo.push_back(0);
        silo[y] |= 1 << (x - 1);
    }
}

static auto fits(const vector<uint8_t>& silo, const set<pair<int, int>>& block) {
    for (const auto& p : block) {
        const auto& [x, y] = p;
        if ((x < 1) || (x > 7) || (y < 0)) return false;
        if (y < silo.size()) {
            const auto& lvl = silo[y];
            if (lvl & (1 << (x - 1))) return false;
        }
    }
    return true;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f{"day17.txt"};
    string gust_direction;
    getline(f, gust_direction);
    vector<uint8_t> silo, cycle;
    uint64_t part1 = 0;
    int cycle_max = 10000;
    uint64_t cycle_start = 0, cycle_offset = 0;
    uint64_t ds = 0, dy1 = 0, dy2 = 0;
    int gust_ix = 0;
    int block_ix = 0;
    uint64_t dropped = 0;
    while (dropped < (3 * cycle_max)) {
        if (dropped == 2022)
            part1 = silo.size();
        else if (dropped == (2 * cycle_max))
            cycle = silo;
        else if ((!cycle_start) && (dropped > (2 * cycle_max))) {
            if (memcmp(&silo[silo.size() - cycle_max], &cycle[cycle.size() - cycle_max],
                       cycle_max * sizeof(uint8_t)) == 0) {
                cycle_start = (dropped - (2 * cycle_max));
                cycle_offset = (1000000000000 % cycle_start);
            }
        } else if (cycle_start && (!dy1) && ((dropped % cycle_start) == cycle_offset))
            dy1 = silo.size();
        else if (cycle_start && dy1 && !ds && ((dropped % cycle_start) == cycle_offset)) {
            ds = dropped;
            dy2 = silo.size();
            break;
        }
        auto b1 = blocks[block_ix] + int(silo.size());
        for (;;) {
            const auto b2 = blow(b1, gust_direction[gust_ix] == '<');
            const auto b3 = fits(silo, b2) ? b2 : b1;
            const auto b4 = (b3 + (-1));
            if ((++gust_ix) == gust_direction.size()) gust_ix = 0;
            if (!fits(silo, b4)) {
                fall(silo, b3);
                break;
            } else
                b1 = b4;
        }
        block_ix = (block_ix + 1) % 5;
        ++dropped;
    }
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << (((1000000000000 - ds) / cycle_start) * (dy2 - dy1)) + dy2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
