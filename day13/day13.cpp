#define title "--- Day 13: Distress Signal ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

struct fragment {
    int number;
    list<fragment> numbers;
};

static fragment parse(stringstream& ss) {
    auto f = new fragment;
    char c = ss.peek();
    if (c == '[') {
        ss >> c;
        f->number = -1;
        for (;;) {
            c = ss.peek();
            if (c == ']') {
                ss >> c;
                break;
            }
            f->numbers.push_back(parse(ss));
            c = ss.peek();
            if (c == ',') ss >> c;
        }
    } else
        ss >> f->number;
    return *f;
}

enum result { good, bad, unknown };
static result compare_fragment(const fragment& m1, const fragment& m2);

static auto compare_fragment_list(const fragment& m1, const fragment& m2) {
    auto im1 = m1.numbers.begin();
    auto im2 = m2.numbers.begin();
    while (im1 != m1.numbers.end() && im2 != m2.numbers.end()) {
        auto r = compare_fragment(*im1, *im2);
        if (r != unknown) return r;
        im1++;
        im2++;
    }
    if (im1 == m1.numbers.end() && im2 == m2.numbers.end()) return unknown;
    if (im1 == m1.numbers.end())
        return good;
    else
        return bad;
}

static result compare_fragment(const fragment& m1, const fragment& m2) {
    if (m1.number >= 0 && m2.number >= 0) {
        if (m1.number < m2.number) return good;
        if (m1.number > m2.number) return bad;
        return unknown;
    }
    if (m1.number < 0 && m2.number < 0) {
        return compare_fragment_list(m1, m2);
    }
    fragment f1 = m1;
    fragment f2 = m2;
    auto f = new fragment;
    if (f1.number >= 0) {
        f->number = f1.number;
        f1.number = -1;
        f1.numbers.push_back(*f);
    } else {
        f->number = f2.number;
        f2.number = -1;
        f2.numbers.push_back(*f);
    }
    return compare_fragment_list(f1, f2);
}

static auto compare(const fragment& m1, const fragment& m2) {
    return compare_fragment(m1, m2) == good;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day13.txt");
    string s;
    int index = 0, sum = 0;
    fragment msg1, msg2;

    stringstream ss;
    list<fragment> msgs;
    while (getline(f, s)) {
        index++;
        ss.str(s);
        msg1 = parse(ss);
        msgs.push_back(msg1);
        getline(f, s);
        ss.str(s);
        msg2 = parse(ss);
        msgs.push_back(msg2);
        getline(f, s);
        if (compare_fragment(msg1, msg2) == good) sum += index;
    }
    ss.str("[[2]]");
    msg1 = parse(ss);
    msgs.push_back(msg1);
    ss.str("[[6]]");
    msg2 = parse(ss);
    msgs.push_back(msg2);
    msgs.sort(compare);
    index = 0;
    int key = 1;
    for (auto& m : msgs) {
        index++;
        if (compare_fragment(m, msg1) == unknown)
            key *= index;
        else if (compare_fragment(m, msg2) == unknown)
            key *= index;
    }
    cout << title << endl
         << "Part 1  - " << sum << endl
         << "Part 2  - " << key << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1e3 << " ms."
         << endl;
}
