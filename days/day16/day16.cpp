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

vector<int16_t> pressure;
vector<int16_t> total_pressure;
vector<vector<int16_t>> to;

int search(int p, int used, int time, bool elephants) {
    if (time == 0)
        return elephants ? search(0, used, 26, !elephants) : 0;
    auto key = used * (pressure.size() * 31 * 2) + (p * 31 * 2) + (time * 2) + (elephants ? 1 : 0);
    if (total_pressure[key] >= 0)
        return total_pressure[key];
    int part = 0;
    if (!(used & (1ULL << p)) && pressure[p]) {
        int new_used = used | (1ULL << p);
        part = max(part, (time - 1) * pressure[p] + search(p, new_used, time - 1, elephants));
    }
    for (auto& to : to[p])
        part = max(part, search(to, used, time - 1, elephants));
    total_pressure[key] = part;
    return part;
}

int main() {
    auto strt = high_resolution_clock::now();
    map<string, pair<int, vector<string>>> volcano;
    ifstream f("day16.txt");
    string line;
    while (getline(f, line)) {
        istringstream ss(line);
        istream_iterator<string> begin(ss), end;
        vector<string> tokens(begin, end);
        string id(tokens[1]);
        int rate(stoi(tokens[4].substr(5)));
        vector<string> to;
        for (int i = 9; i < tokens.size(); ++i)
            to.push_back(tokens[i].substr(0, 2));
        volcano[id] = make_pair(rate, to);
    }
    int n = int(volcano.size());
    map<string, int> map_index;
    vector<string> map_order;
    int with_pressure = 0;
    for (auto& p : volcano)
        if (p.first == "AA") {
            map_index[p.first] = int(map_order.size());
            map_order.push_back(p.first);
            with_pressure++;
        }
    for (auto& p : volcano)
        if (p.second.first > 0) {
            map_index[p.first] = int(map_order.size());
            map_order.push_back(p.first);
            with_pressure++;
        }
    for (auto& p : volcano)
        if (map_index.count(p.first) == 0) {
            map_index[p.first] = int(map_order.size());
            map_order.push_back(p.first);
        }
    pressure = vector<int16_t>(n, 0);
    for (int i = 0; i < n; i++)
        pressure[i] = volcano[map_order[i]].first;
    to = vector<vector<int16_t>>(n, vector<int16_t>{});
    for (int i = 0; i < n; i++)
        for (auto& j : volcano[map_order[i]].second)
            to[i].push_back(map_index[j]);
    volcano.clear();
    map_index.clear();
    map_order.clear();
    total_pressure = vector<int16_t>((1 << with_pressure) * n * 31 * 2, -1);
    cout << title << endl
         << "Part 1  - " << search(0, 0, 30, false) << endl
         << "Part 2  - " << search(0, 0, 26, true) << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
