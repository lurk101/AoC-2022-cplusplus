#define title "--- Day 12: Hill Climbing Algorithm ---"
#include <chrono>
#include <climits>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

struct point {
    int y, x, dist;
    point operator+(const point& p) { return {y + p.y, x + p.x}; }
    auto outside(const point dim) { return x < 0 || x >= dim.x || y < 0 || y >= dim.y; }
};

static vector<vector<char>> terrain;
static point start, finish, dim;

static const point dydx[] = {{0, -1}, {-1, 0}, {1, 0}, {0, 1}};

static auto search(void) {
    vector<vector<int>> visited(dim.y, vector<int>(dim.x, 0));
    queue<point> q;
    start.dist = 0;
    q.push(start);
    while (!q.empty()) {
        auto p = q.front();
        q.pop();
        if (p.x == finish.x && p.y == finish.y) continue;
        for (const auto& d : dydx) {
            point p2(p + d);
            if (p2.outside(dim) || terrain[p2.y][p2.x] - terrain[p.y][p.x] > 1 ||
                visited[p2.y][p2.x])
                continue;
            p2.dist = p.dist + 1;
            visited[p2.y][p2.x] = p2.dist;
            q.push(p2);
        }
    }
    return visited[finish.y][finish.x];
};

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day12.txt");
    string s;
    while (getline(f, s)) {
        vector<char> m;
        for (char c : s) {
            if (c == 'S') {
                start = {int(terrain.size()), int(m.size()), 0};
                c = 'a';
            } else if (c == 'E') {
                finish = {int(terrain.size()), int(m.size()), 0};
                c = 'z';
            }
            m.push_back(c);
        }
        terrain.push_back(m);
    }
    dim = {int(terrain.size()), int(terrain[0].size())};
    cout << title << endl << "Part 1  - " << search() << endl;
    int best = INT_MAX;
    for (int y = 0; y < dim.y; y++)
        for (int x = 0; x < dim.x; x++)
            if (terrain[y][x] == 'a') {
                start = {y, x};
                int l = search();
                if (l && l < best) best = l;
            }
    cout << "Part 2  - " << best << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
