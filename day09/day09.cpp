#define title "--- Day 9: Rope Bridge ---"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>

using namespace std;
using namespace chrono;

struct knot {
    int x, y;
};
typedef struct knot knot_t;

static set<knot_t> knots2;
static set<knot_t> knots10;

static const int knots = 10;

static auto operator<(const knot& lhs, const knot& rhs) {
    if (lhs.x == rhs.x) return lhs.y < rhs.y;
    return lhs.x < rhs.x;
}

static void follow_next(const knot_t& head, knot_t& tail) {
    if (head.x == tail.x) {
        if (abs(head.y - tail.y) < 2) return;
        if (tail.y < head.y)
            ++tail.y;
        else
            --tail.y;
        return;
    } else if (head.y == tail.y) {
        if (abs(head.x - tail.x) < 2) return;
        if (tail.x < head.x)
            ++tail.x;
        else
            --tail.x;
        return;
    } else {
        if ((abs(head.y - tail.y) < 2) && (abs(head.x - tail.x) < 2)) return;
        if (tail.y < head.y)
            ++tail.y;
        else
            --tail.y;
        if (tail.x < head.x)
            ++tail.x;
        else
            --tail.x;
    }
}

static void follow(const knot_t& head, knot_t (&tail)[knots - 1]) {
    follow_next(head, tail[0]);
    knots2.insert(tail[0]);
    for (int i = 1; i < knots - 1; i++) follow_next(tail[i - 1], tail[i]);
    knots10.insert(tail[knots - 2]);
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day09.txt");
    knot_t head = {0}, tail[knots - 1] = {0};
    char dir;
    int dist;
    knots2.insert(tail[0]);
    knots10.insert(tail[0]);
    while (f >> dir >> dist) {
        switch (dir) {
        case 'U':
            for (int i = 0; i < dist; ++i) {
                ++head.y;
                follow(head, tail);
            }
            break;
        case 'D':
            for (int i = 0; i < dist; ++i) {
                --head.y;
                follow(head, tail);
            }
            break;
        case 'L':
            for (int i = 0; i < dist; ++i) {
                --head.x;
                follow(head, tail);
            }
            break;
        case 'R':
            for (int i = 0; i < dist; ++i) {
                ++head.x;
                follow(head, tail);
            }
            break;
        }
    }
    cout << title << endl
         << "Part 1  - " << knots2.size() << endl
         << "Part 2  - " << knots10.size() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
