#define title "--- Day 24: Blizzard Basin ---"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

struct point {
    int x, y;
    auto operator<(const point& p) const {
        if (y == p.y) return x < p.x;
        return y < p.y;
    }
    auto operator+(const point& p) const { return point({x + p.x, y + p.y}); }
    auto operator==(const point& p) const { return x == p.x && y == p.y; }
};

static const point directions[5] = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}, {0, 0}};

static auto in_range(int min, int val, int max) { return min <= val && val <= max; }

struct box {
    int min_x, min_y, max_x, max_y;
    box(int x, int y, int X, int Y) : min_x(x), min_y(y), max_x(X), max_y(Y) {}
    auto inside(const point& p) const {
        return in_range(min_x, p.x, max_x) && in_range(min_y, p.y, max_y);
    }
};

static vector<point> blizzards, up_blizzards, down_blizzards, left_blizzards, right_blizzards;
static int max_x, max_y;
static point start, target;
static box bounds({0, 0, 0, 0});

static auto traverse(point a, point b) {
    int t = 0;
    vector<point> points = {a}, next_points;
    while (!binary_search(points.begin(), points.end(), b)) {
        blizzards.clear();
        for (point& p : up_blizzards) {
            if (!--p.x) p.x = max_x - 1;
            blizzards.push_back(p);
        }
        for (point& p : left_blizzards) {
            if (!--p.y) p.y = max_y - 1;
            blizzards.push_back(p);
        }
        for (point& p : down_blizzards) {
            if (++p.x == max_x) p.x = 1;
            blizzards.push_back(p);
        }
        for (point& p : right_blizzards) {
            if (++p.y == max_y) p.y = 1;
            blizzards.push_back(p);
        }
        sort(blizzards.begin(), blizzards.end());
        blizzards.erase(unique(blizzards.begin(), blizzards.end()), blizzards.end());
        for (auto& p : points)
            for (const auto& d : directions) {
                point n = p + d;
                if ((bounds.inside(n) || n == start || n == target) &&
                    !binary_search(blizzards.begin(), blizzards.end(), n))
                    next_points.push_back(n);
            }
        sort(next_points.begin(), next_points.end());
        next_points.erase(unique(next_points.begin(), next_points.end()), next_points.end());
        points = next_points;
        next_points.clear();
        ++t;
    }
    return t;
};

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day24.txt");
    string line;
    while (f >> line) {
        if (!max_x++) {
            start = {0, int(line.find('.'))};
            max_y = int(line.size()) - 1;
        } else
            for (int i = 1, x = max_x - 1; i < line.size() - 1; ++i) switch (line[i]) {
                case '>':
                    right_blizzards.push_back({x, i});
                    break;
                case '<':
                    left_blizzards.push_back({x, i});
                    break;
                case '^':
                    up_blizzards.push_back({x, i});
                    break;
                case 'v':
                    down_blizzards.push_back({x, i});
                    break;
                case '.':
                    target = {x, i};
                }
    }
    bounds = {1, 1, --max_x - 1, max_y - 1};
    int part1 = 0, part2 = 0;
    part2 = part1 = traverse(start, target);
    part2 += traverse(target, start);
    part2 += traverse(start, target);
    cout << title << endl
         << "Part 1  - " << part1 << endl
         << "Part 2  - " << part2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
