#define title "--- Day 19: Not Enough Minerals ---"
#include <chrono>
#include <fstream>
#include <future>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

static const uint32_t Ore = 1 << 0, Clay = 1 << 8, Obsidian = 1 << 16, Geode = 1 << 24;

static uint32_t ore(uint32_t a) { return a & 0xff; }
static uint32_t clay(uint32_t a) { return (a >> 8) & 0xff; }
static uint32_t obsidian(uint32_t a) { return (a >> 16) & 0xff; }
static uint32_t geode(uint32_t a) { return (a >> 24) & 0xff; }

struct blueprint {
    uint32_t id, ore, clay, obsidian_ore, obsidian_clay, geode_ore, geode_clay;
};

static auto to_arrival(uint32_t need, uint32_t have) { return (need + have - 1) / have; }

static uint32_t search(blueprint& bp, uint32_t assets, uint32_t bot, int t) {
    uint32_t best = geode(assets + t * bot);
    if (ore(bot) < bp.clay || ore(bot) < bp.obsidian_ore || ore(bot) < bp.geode_ore) {
        int dt = 1 + (ore(assets) > bp.ore ? 0 : to_arrival(bp.ore - ore(assets), ore(bot)));
        if (t > dt)
            best = max(best, search(bp, assets + dt * bot - bp.ore * Ore, bot + Ore, t - dt));
    }
    if (clay(bot) < bp.obsidian_clay) {
        int dt = 1 + (ore(assets) > bp.clay ? 0 : to_arrival(bp.clay - ore(assets), ore(bot)));
        if (t > dt)
            best = max(best, search(bp, assets + dt * bot - bp.clay * Ore, bot + Clay, t - dt));
    }
    if (clay(bot)) {
        uint32_t ore_n =
            ore(assets) > bp.obsidian_ore ? 0 : to_arrival(bp.obsidian_ore - ore(assets), ore(bot));
        uint32_t clay_n = clay(assets) > bp.obsidian_clay
                              ? 0
                              : to_arrival(bp.obsidian_clay - clay(assets), clay(bot));
        int dt = 1 + max(ore_n, clay_n);
        if (t > dt)
            best =
                max(best,
                    search(bp, assets + dt * bot - bp.obsidian_ore * Ore - bp.obsidian_clay * Clay,
                           bot + Obsidian, t - dt));
    }
    if (obsidian(bot)) {
        int dt =
            1 +
            max(ore(assets) > bp.geode_ore ? 0 : to_arrival(bp.geode_ore - ore(assets), ore(bot)),
                obsidian(assets) > bp.geode_clay
                    ? 0
                    : to_arrival(bp.geode_clay - obsidian(assets), obsidian(bot)));
        if (t > dt)
            best = max(best,
                       search(bp, assets + dt * bot - bp.geode_ore * Ore - bp.geode_clay * Obsidian,
                              bot + Geode, t - dt));
    }
    return best;
}

static vector<blueprint> blueprints;

static int prt2(int bp_ix) { return search(blueprints[bp_ix], 0, Ore, 32); }

int main(void) {
    auto strt = high_resolution_clock::now();
    ifstream f("day19.txt");
    string line;
    while (getline(f, line)) {
        blueprint bp{};
        (void)sscanf(line.c_str(),
                     "Blueprint %d: Each ore robot costs %d ore. "
                     "Each clay robot costs %d ore. "
                     "Each obsidian robot costs %d ore and %d clay. "
                     "Each geode robot costs %d ore and %d obsidian",
                     &bp.id, &bp.ore, &bp.clay, &bp.obsidian_ore, &bp.obsidian_clay, &bp.geode_ore,
                     &bp.geode_clay);
        blueprints.push_back(bp);
    }
    auto f1 = async(launch::async, prt2, 0);
    auto f2 = async(launch::async, prt2, 1);
    auto f3 = async(launch::async, prt2, 2);
    uint32_t part1 = 0;
    for (int i = 0; i < blueprints.size(); i++)
        part1 += blueprints[i].id * search(blueprints[i], 0, Ore, 24);
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << f1.get() * f2.get() * f3.get() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
