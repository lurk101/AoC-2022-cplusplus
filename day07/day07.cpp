#define title "--- Day 7: No Space Left On Device ---"
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <sstream>
#include <string>

using namespace std;
using namespace chrono;

struct node {
    list<node> dirs;
    node* parent;
    string name;
    int size = 0;
};

static struct node root, *cur_dir;

static void change_dir(string d) {
    if (d == "/") {
        cur_dir = &root;
        cur_dir->name = "/";
        cur_dir->parent = nullptr;
        return;
    }
    if (d == "..") {
        cur_dir = cur_dir->parent;
        return;
    }
    for (auto& n : cur_dir->dirs) {
        if (n.name == d) {
            cur_dir = &n;
            break;
        }
    }
    return;
}

static int dir_size(struct node& dir) {
    for (auto& n : dir.dirs) dir.size += dir_size(n);
    return dir.size;
}

static void dir_at_most(struct node& dir, int& sum, int& size) {
    if (dir.size <= 100000) sum += dir.size;
    int need = 30000000 - (70000000 - root.size);
    if (dir.size >= need)
        if (dir.size < size) size = dir.size;
    for (auto& n : dir.dirs) dir_at_most(n, sum, size);
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day07.txt");
    string l;
    while (getline(f, l)) {
        if (l.substr(0, 5) == "$ cd ")
            change_dir(l.substr(5));
        else if (l.substr(0, 4) == "$ ls")
            continue;
        else if (l.substr(0, 4) == "dir ") {
            struct node* n = new (node);
            n->name = l.substr(4);
            n->parent = cur_dir;
            cur_dir->dirs.push_back(*n);
        } else {
            int size = atoi(l.c_str());
            cur_dir->size += size;
        }
    }
    dir_size(root);
    int sum = 0;
    int smallest = 0x7fffffff;
    dir_at_most(root, sum, smallest);
    cout << title << endl
         << "Part 1 - " << sum << endl
         << "Part 2 - " << smallest << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
