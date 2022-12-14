#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <list>
#include <regex>
#include <string>

using namespace std;
using namespace filesystem;

float run_once(bool display) {
    FILE* fp = popen("./a.out", "r");
    char out[256];
    float elapsed;
    while (fgets(out, sizeof(out), fp) != NULL) {
        if (!strncmp(out, "Elapsed - ", 10))
            elapsed = strtof(out + 10, NULL);
        if (display)
            cout << out;
    }
    pclose(fp);
    return elapsed;
}

float run_many(void) {
    float best = run_once(true);
    int repeats = 0, limit = 10;
    for (;;) {
        if (++repeats > limit)
            break;
        float e = run_once(false);
        if (e < best) {
            best = e;
            repeats = 0;
        }
    }
    return best;
}

list<float> results;

void run_test(string& path) {
    cout << "Running " << path << endl;
    copy_file(path + "/" + path + ".cpp", path + ".cpp", copy_options::overwrite_existing);
    copy_file(path + "/" + path + ".txt", path + ".txt", copy_options::overwrite_existing);
    system(("g++ -O3 -std=c++17 " + path + ".cpp").c_str());
    results.push_back(run_many());
    cout << "Best : " << results.back() << endl;
    system(("rm " + path + ".txt " + path + ".cpp a.out").c_str());
}

int main(int ac, char** av) {
    if (ac < 2) {
        cout << "missing run name" << endl;
        exit(-1);
    }
    list<string> paths;
    const string path = ".";
    regex e("./day[0-9]+");
    if (ac > 2)
        e = string("./day") + av[2];
    smatch m;
    for (const auto& entry : directory_iterator(path)) {
        string p(entry.path());
        if (regex_search(p, m, e))
            paths.push_back(p.substr(2));
    }
    paths.sort();
    cout << "Runnung:";
    for (auto& p : paths)
        cout << ' ' << p;
    cout << endl;
    for (auto& p : paths)
        run_test(p);
    ofstream o(av[1]);
    int i = 0;
    while (results.size()) {
        o << ++i << ' ' << results.front() << endl;
        results.pop_front();
    }
    o.close();
}
