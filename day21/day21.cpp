#define title "--- Day 21: Monkey Math ---"
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

struct monkey {
    char op;
    string operand[2];
};

static map<string, int64_t> values;
static map<string, monkey> monkeys;
static map<string, string> parents;

static auto evaluate(const string& name) {
    if (values.count(name))
        return values[name];
    else {
        auto mky = monkeys[name];
        switch (mky.op) {
        case '+':
            return evaluate(mky.operand[0]) + evaluate(mky.operand[1]);
        case '-':
            return evaluate(mky.operand[0]) - evaluate(mky.operand[1]);
        case '/':
            return evaluate(mky.operand[0]) / evaluate(mky.operand[1]);
        case '*':
            return evaluate(mky.operand[0]) * evaluate(mky.operand[1]);
        };
    }
    return int64_t(0);
}

static int64_t needs(const string& name, const string& child) {
    if (name == "humn")
        return needs(parents[name], name);
    else {
        auto mky = monkeys[name];
        string known = mky.operand[0] == child ? mky.operand[1] : mky.operand[0];
        if (name == "root")
            return evaluate(known);
        else {
            int64_t operand = evaluate(known);
            int64_t wanted = needs(parents[name], name);
            switch (mky.op) {
            case '+':
                return wanted - operand;
            case '-':
                if (child == mky.operand[0])
                    return operand + wanted;
                else
                    return operand - wanted;
            case '*':
                return wanted / operand;
            case '/':
                if (child == mky.operand[0])
                    return operand * wanted;
                else
                    return operand / wanted;
            }
        }
    }
    return 0L;
}

int main() {
    auto strt = high_resolution_clock::now();
    ifstream f("day21.txt");
    string line;
    while (getline(f, line)) {
        istringstream ss(line);
        istream_iterator<string> begin(ss), end;
        vector<string> tokens(begin, end);
        if (tokens.size() == 2)
            values[tokens[0].substr(0, 4)] = stoi(tokens[1]);
        else {
            monkey op({tokens[2][0], tokens[1], tokens[3]});
            monkeys[tokens[0].substr(0, 4)] = op;
            parents[tokens[1]] = tokens[0].substr(0, 4);
            parents[tokens[3]] = tokens[0].substr(0, 4);
        }
    }
    cout << title << endl
         << "Part 1  - " << evaluate("root") << endl
         << "Part 2  - " << needs("humn", "") << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - strt).count() / 1.0e3
         << " ms." << endl;
}
