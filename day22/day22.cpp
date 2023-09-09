#define title "--- Day 22: Monkey Map ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;
using namespace chrono;

enum state { vacuum, emptee, wall };

struct point {
    int x, y;
    auto operator+(const point& r) const { return point({x + r.x, y + r.y}); }
    auto operator<(const point& r) const { return y == r.y ? x < r.x : y < r.y; }
};

static const int board_height = 202;
static const int board_width = 152;

static vector<vector<state>> board(board_height, vector<state>(board_width, vacuum));

static string instructions;

static point directions[4] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

static auto get_first() {
    point p{1, 1};
    while (board[p.x][p.y] == vacuum) p = p + directions[0];
    return p;
}

static auto part1() {
    int direction = 0;
    point p(get_first());
    int instr_ix = 0;
    while (instr_ix != instructions.length()) {
        auto next_ix = instr_ix;
        while (instr_ix < instructions.length() && isdigit(instructions[next_ix])) next_ix++;
        int steps = stoi(instructions.substr(instr_ix, (next_ix - instr_ix)));
        for (int i = 0; i < steps; ++i) {
            point next = p + directions[direction];
            bool end = false;
            switch (board[next.x][next.y]) {
            case vacuum: {
                auto o_dir = (direction + 2) % 4;
                point o_next = p + directions[o_dir];
                while (o_next.y >= 0 && o_next.x >= 0 && o_next.x < board_height &&
                       o_next.y < board_width && board[o_next.x][o_next.y] != vacuum) {
                    o_next = o_next + directions[o_dir];
                }
                o_next = o_next + directions[direction];
                if (board[o_next.x][o_next.y] == emptee)
                    p = o_next;
                else if (board[o_next.x][o_next.y] == wall) {
                    end = true;
                    break;
                }
                break;
            }
            case emptee:
                p = next;
                break;
            case wall:
                end = true;
                break;
            }
            if (end) break;
        }
        instr_ix = next_ix;
        if (instr_ix < instructions.length())
            direction = (direction + ((instructions[instr_ix++] == 'R') ? 1 : 3)) % 4;
    }
    return 1000 * (p.x) + 4 * (p.y) + direction;
}

static auto part2() {
    int direction = 0;
    point p(get_first());
    map<point, int> sides;
    int side_len = 50;
    int side_ix = 1;
    for (int x = 1; x < board_height; x += side_len)
        for (int y = 1; y < board[x].size(); y += side_len)
            if (board[x][y] != vacuum) {
                for (int xx = x; xx < x + side_len; ++xx)
                    for (int yy = y; yy < y + side_len; ++yy) sides[{int(xx), int(yy)}] = side_ix;
                ++side_ix;
            }
    int instr_ix = 0;
    vector<string> paths(board_height, string{});
    for (int x = 0; x < board_height; ++x)
        for (int y = 0; y < board[x].size(); ++y)
            paths[x] += board[x][y] == vacuum ? ' ' : board[x][y] == emptee ? '.' : '#';
    auto get_next = [&sides](const point& current, int direction) -> tuple<point, int, bool> {
        point next = current + directions[direction];
        int next_dir = direction;
        switch (board[next.x][next.y]) {
        case vacuum:
            switch (sides[current]) {
            case 1:
                switch (direction) {
                case 2:
                    next = {151 - current.x, 1};
                    next_dir = 0;
                    break;
                case 3:
                    next_dir = 0;
                    next = {150 + (current.y - 50), 1};
                    break;
                }
                break;
            case 2:
                switch (direction) {
                case 0:
                    next_dir = 2;
                    next = {151 - current.x, 100};
                    break;
                case 1:
                    next_dir = 2;
                    next = {50 + (current.y - 100), 100};
                    break;
                case 3:
                    next_dir = 3;
                    next = {200, current.y - 100};
                    break;
                }
                break;
            case 3:
                switch (direction) {
                case 0:
                    next_dir = 3;
                    next = {50, 100 + (current.x - 50)};
                    break;
                case 2:
                    next_dir = 1;
                    next = {101, current.x - 50};
                    break;
                default:
                    break;
                }
                break;
            case 4:
                switch (direction) {
                case 2:
                    next_dir = 0;
                    next = {51 - (current.x - 100), 51};
                    break;
                case 3:
                    next_dir = 0;
                    next = {50 + current.y, 51};
                    break;
                }
                break;
            case 5:
                switch (direction) {
                case 0:
                    next_dir = 2;
                    next = {151 - current.x, 150};
                    break;
                case 1:
                    next_dir = 2;
                    next = {150 + (current.y - 50), 50};
                    break;
                }
                break;
            case 6:
                switch (direction) {
                case 0:
                    next_dir = 3;
                    next = {150, 50 + (current.x - 150)};
                    break;
                case 1:
                    next_dir = 1;
                    next = {1, 100 + current.y};
                    break;
                case 2:
                    next_dir = 1;
                    next = {1, 50 + (current.x - 150)};
                    break;
                }
                break;
            }
            if (board[next.x][next.y] == wall) return make_tuple(current, direction, true);
            return make_tuple(next, next_dir, false);
        case emptee:
            return make_tuple(next, direction, false);
        case wall:
            return make_tuple(current, direction, true);
        }
        return make_tuple(point({0, 0}), 0, false);
    };
    char symbols[4] = {'>', 'v', '<', '^'};
    while (instr_ix != instructions.length()) {
        auto next_ix = instr_ix;
        while (instr_ix < instructions.length() && isdigit(instructions[next_ix])) next_ix++;
        int steps = atoi(instructions.substr(instr_ix, (next_ix - instr_ix)).c_str());
        for (int i = 0; i < steps; ++i) {
            paths[p.x][p.y] = symbols[direction];
            bool end;
            tie(p, direction, end) = get_next(p, direction);
            if (end) break;
        }
        instr_ix = next_ix;
        if (instr_ix < instructions.length())
            direction = (direction + ((instructions[instr_ix++] == 'R') ? 1 : 3)) % 4;
    }
    return 1000 * (p.x) + 4 * (p.y) + direction;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day22.txt");
    vector<string> lines;
    string line;
    int y = 1;
    while (getline(f, line)) {
        if (line == "") break;
        int x = 1;
        for (auto& c : line) {
            switch (c) {
            case '.':
                board[y][x] = emptee;
                break;
            case '#':
                board[y][x] = wall;
                break;
            }
            ++x;
        }
        ++y;
    }
    getline(f, instructions);
    cout << title << endl
         << "Part 1  - " << part1() << endl
         << "Part 2  - " << part2() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
