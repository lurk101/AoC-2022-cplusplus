#define title "--- Day 15: Beacon Exclusion Zone ---"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace std;
using namespace chrono;

#define Y 2000000

struct sensor_s {
    int x, y, dist;
    sensor_s(int sx, int sy, int dst) : x(sx), y(sy), dist(dst){};
};

struct intvl_s {
    int64_t l, r;
    auto operator<(const intvl_s& intvl) const {
        if (l == intvl.l) return r < intvl.r;
        return l < intvl.l;
    }
};

static vector<string> lines;
static int p1;
static int64_t p2;

static void part1(void) {
    vector<intvl_s> intvls;
    for (auto& line : lines) {
        int sx, sy, bx, by;
        if (sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy,
                   &bx, &by) == 4) {
            int dx = abs(sx - bx), dy = abs(sy - by), dist = dx + dy;
            int64_t diff_y = abs(sy - Y), dist_left = dist - diff_y;
            if (dist_left < 0) continue;
            intvl_s intvl = {sx - dist_left, sx + dist_left};
            intvls.push_back(intvl);
        }
    }
    sort(intvls.begin(), intvls.end());
    auto cur_intvl = intvls[0];
    intvls.erase(intvls.begin());
    int64_t count = 0;
    for (auto intvl : intvls) {
        if (cur_intvl.l <= intvl.l && intvl.r <= cur_intvl.r)
            continue;
        else if (cur_intvl.l <= intvl.l && cur_intvl.r <= intvl.r && intvl.l <= cur_intvl.r)
            cur_intvl.r = intvl.r;
        else {
            count += (cur_intvl.r - cur_intvl.l) + 1;
            cur_intvl = intvl;
        }
    }
    p1 = int(count + cur_intvl.r - cur_intvl.l);
}

#define MAX_VAL 4000000

static void part2(void) {
    vector<sensor_s> sensors;
    for (auto& line : lines) {
        int sx, sy, bx, by;
        if (sscanf(line.c_str(), "Sensor at x=%d, y=%d: closest beacon is at x=%d, y=%d", &sx, &sy,
                   &bx, &by) == 4) {
            int dist = abs(sx - bx) + abs(sy - by);
            sensors.push_back(sensor_s{sx, sy, dist});
        }
    }
    for (int64_t cur_y = 0; cur_y <= MAX_VAL; cur_y++) {
        vector<intvl_s> intvls;
        for (sensor_s sensor : sensors) {
            int64_t diff_y = abs(sensor.y - cur_y), dist_left = sensor.dist - diff_y;
            if (dist_left < 0) continue;
            intvl_s intvl = {max(sensor.x - dist_left, int64_t(0)),
                             min(sensor.x + dist_left, int64_t(MAX_VAL))};
            intvls.push_back(intvl);
        }
        sort(intvls.begin(), intvls.end());
        intvl_s cur_intvl = intvls[0];
        intvls.erase(intvls.begin());
        for (intvl_s intvl : intvls) {
            if (cur_intvl.l <= intvl.l && intvl.r <= cur_intvl.r)
                continue;
            else if (cur_intvl.l <= intvl.l && cur_intvl.r <= intvl.r && intvl.l <= cur_intvl.r)
                cur_intvl.r = intvl.r;
            else {
                p2 = cur_y + MAX_VAL * (cur_intvl.r + 1);
                return;
            }
        }
    }
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day15.txt");
    string line;
    while (getline(f, line)) lines.push_back(line);
    thread t1(part1);
    thread t2(part2);
    t1.join();
    t2.join();
    cout << title << endl
         << "Part 1  - " << p1 << endl
         << "Part 2  - " << p2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
