#define title "--- 5: Supply Stacks ---"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace chrono;

static vector<char> stack[10], stack2[10];

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day05.txt");
    string l;
    while (getline(f, l)) {
        if (l[1] == '1') {
            getline(f, l);
            break;
        }
        int len = l.length();
        for (int i = 1; i < 10; ++i) {
            if (((i * 4) - 3) >= len) break;
            char c = l[(static_cast<std::basic_string<char, std::char_traits<char>, std::allocator<char>>::size_type>(i) * 4) - 3];
            if (c != ' ') stack[i].push_back(c);
        }
    }
    for (int i = 1; i < 10; ++i) {
        reverse(stack[i].begin(), stack[i].end());
        stack2[i] = stack[i];
    }
    int n, from, to;
    string s;
    while (f >> s >> n >> s >> from >> s >> to) {
        for (int j = 0; j < n; ++j) {
            stack[to].push_back(stack[from].back());
            stack[from].pop_back();
        }
        vector<char> temp;
        for (int j = 0; j < n; ++j) {
            temp.push_back(stack2[from].back());
            stack2[from].pop_back();
        }
        for (int j = 0; j < n; ++j) {
            stack2[to].push_back(temp.back());
            temp.pop_back();
        }
    }
    cout << title << endl << "Part 1 - ";
    for (int i = 1; i < 10; i++)
        if (stack[i].size()) cout << stack[i].back();
    cout << endl << "Part 2 - ";
    for (int i = 1; i < 10; i++)
        if (stack2[i].size()) cout << stack2[i].back();
    cout << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
