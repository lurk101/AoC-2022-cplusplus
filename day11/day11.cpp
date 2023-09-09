#define title "--- Day 11: Monkey in the Middle ---"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

uint32_t multiple = 1;

struct monkey_s {
    list<uint64_t> items;
    uint32_t oper = 0, oprnd, divby, trueTo, falseTo, visits;

    auto add(uint64_t w) -> void { items.push_back(w); }
};

static void turn(monkey_s& m, int part);

struct barrel_s {
    vector<monkey_s> ms;
    vector<monkey_s> save_ms;

    auto add(monkey_s& m) -> void { ms.push_back(m); }

    auto add_to(uint32_t to, uint64_t w) -> void { ms[to].add(w); }

    auto biz(void) {
        uint64_t x = 0, y = 0;
        for (auto& m : ms) {
            if (m.visits > x) {
                y = x;
                x = m.visits;
            } else if (m.visits > y)
                y = m.visits;
        }
        return x * y;
    }

    void round(int part) {
        for (auto& m : ms) turn(m, part);
    }

    void save(void) { save_ms = ms; }

    void restore(void) { ms = save_ms; }
};

static barrel_s monkeys;

static void turn(monkey_s& m, int part) {
    m.visits += uint32_t(m.items.size());
    while (m.items.size()) {
        uint64_t worry = m.items.front();
        m.items.pop_front();
        switch (m.oper) {
        case '+':
            worry += m.oprnd;
            break;
        case '*':
            worry *= m.oprnd;
            break;
        case '-':
            worry += worry;
            break;
        case ',':
            worry *= worry;
            break;
        }
        if (part == 1)
            worry /= 3;
        else
            worry %= multiple;
        if (worry % m.divby == 0)
            monkeys.add_to(m.trueTo, worry);
        else
            monkeys.add_to(m.falseTo, worry);
    }
}

static auto split(const string& s) {
    stringstream ss(s);
    vector<string> r;
    string rs;
    while (ss >> rs) r.push_back(rs);
    return r;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day11.txt");
    string s;
    while (getline(f, s)) {
        getline(f, s);
        vector<string> v = split(s);
        monkey_s m;
        for (int i = 2; i < v.size(); i++) m.items.push_back(stoi(v[i]));
        getline(f, s);
        v = split(s);
        m.oper = v[4][0];
        if (v[5] == "old")
            m.oper += 2;
        else
            m.oprnd = stoi(v[5]);
        getline(f, s);
        v = split(s);
        m.divby = stoi(v[3]);
        multiple *= m.divby;
        getline(f, s);
        v = split(s);
        m.trueTo = stoi(v[5]);
        getline(f, s);
        v = split(s);
        m.falseTo = stoi(v[5]);
        getline(f, s);
        m.visits = 0;
        monkeys.add(m);
    }
    monkeys.save();
    for (int r = 0; r < 20; ++r) monkeys.round(1);
    uint64_t m_biz1 = monkeys.biz();
    monkeys.restore();
    for (int r = 0; r < 10000; ++r) monkeys.round(2);
    cout << title << endl
         << "Part 1  - " << m_biz1 << endl
         << "Part 2  - " << monkeys.biz() << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
