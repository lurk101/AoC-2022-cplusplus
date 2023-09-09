#define title "--- Day 2: Rock Paper Scissor ---"
#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;
using namespace chrono;

static map<string, int> scores = {
    {"A X", 1 + 3},  // rock, rock, draw
    {"A Y", 2 + 6},  // rock, paper, win
    {"A Z", 3 + 0},  // rock, scissor, loss
    {"B X", 1 + 0},  // paper, rock, loss
    {"B Y", 2 + 3},  // paper, paper, draw
    {"B Z", 3 + 6},  // paper, scissor, win
    {"C X", 1 + 6},  // scissor, rock, win
    {"C Y", 2 + 0},  // scissor, paper, loss
    {"C Z", 3 + 3},  // scissor, scissor, draw
};

static map<char, char> win{{'A', 'Y'}, {'B', 'Z'}, {'C', 'X'}};
static map<char, char> lose{{'A', 'Z'}, {'B', 'X'}, {'C', 'Y'}};
static map<char, char> draw{{'A', 'X'}, {'B', 'Y'}, {'C', 'Z'}};

static string line;

static auto round(int part) {
    if (part == 2) {
        switch (line[2]) {
        case 'X':  // lose
            line[2] = lose[line[0]];
            break;
        case 'Y':  // draw
            line[2] = draw[line[0]];
            break;
        case 'Z':  // win
            line[2] = win[line[0]];
            break;
        }
    }
    return scores[line];
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day02.txt");
    int score1 = 0, score2 = 0;
    while (getline(f, line)) {
        score1 += round(1);
        score2 += round(2);
    }
    cout << title << endl
         << "Part 1 - " << score1 << endl
         << "Part 2 - " << score2 << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0
         << " ms." << endl;
}
