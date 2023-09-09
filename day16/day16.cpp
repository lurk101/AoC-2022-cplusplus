#define title "--- Day 16: Proboscidea Volcanium ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<int16_t> flow;
static vector<int16_t> total_flow;
static vector<vector<int16_t>> to;

static int search(int p, int used, int time, bool elephants) {
    if (time == 0) return elephants ? search(0, used, 26, !elephants) : 0;
    auto key = used * (flow.size() * 31 * 2) + (p * 31 * 2) + (time * 2) + (elephants ? 1 : 0);
    if (total_flow[key] >= 0) return total_flow[key];
    int part = 0;
    if (!(used & (1ULL << p)) && flow[p]) {
        int new_used = used | (1ULL << p);
        part = max(part, (time - 1) * flow[p] + search(p, new_used, time - 1, elephants));
    }
    for (auto& to : to[p]) part = max(part, search(to, used, time - 1, elephants));
    total_flow[key] = part;
    return part;
}

int main() {
    auto strt = high_resolution_clock::now();
    map<string, pair<int, vector<string>>> valves;
    ifstream f("day16.txt");
    string line;
    while (getline(f, line)) {
        istringstream ss(line);
        istream_iterator<string> begin(ss), end;
        vector<string> tokens(begin, end);
        string id(tokens[1]);
        vector<string> to;
        for (int i = 9; i < tokens.size(); ++i) to.push_back(tokens[i].substr(0, 2));
        valves[id] = make_pair(stoi(tokens[4].substr(5)), to);
    }
    int n = int(valves.size());
    map<string, int> map_index;
    vector<string> map_order;
    int with_flow = 0;
    for (auto& p : valves)
        if (p.first == "AA") {
            map_index[p.first] = int(map_order.size());
            map_order.push_back(p.first);
            with_flow++;
        }
    for (auto& p : valves)
        if (p.second.first > 0) {
            map_index[p.first] = int(map_order.size());
            map_order.push_back(p.first);
            with_flow++;
        }
    for (auto& p : valves)
        if (map_index.count(p.first) == 0) {
            map_index[p.first] = int(map_order.size());
            map_order.push_back(p.first);
        }
    flow = vector<int16_t>(n, 0);
    for (int i = 0; i < n; i++) flow[i] = valves[map_order[i]].first;
    to = vector<vector<int16_t>>(n, vector<int16_t>{});
    for (int i = 0; i < n; i++)
        for (auto& j : valves[map_order[i]].second) to[i].push_back(map_index[j]);
    valves.clear();
    map_index.clear();
    map_order.clear();
    total_flow = vector<int16_t>((1 << with_flow) * n * 31 * 2, -1);
    cout << title << endl
         << "Part 1  - " << search(0, 0, 30, false) << endl
         << "Part 2  - " << search(0, 0, 26, true) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
