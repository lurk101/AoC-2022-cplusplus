static const char* title = "--- Day 8: Treetop Tree House ---";
#include <chrono>
#include <fstream>
#include <iostream>

using namespace std;
using namespace chrono;

static const int DIM = 99;

static char grid[DIM][DIM];

static auto is_visible(int row, int col) {
    char h = grid[row][col];
    bool v = true;
    for (int c = col - 1; c >= 0; c--)
        if (grid[row][c] >= h) {
            v = false;
            break;
        }
    if (v) return 1;
    v = true;
    for (int c = col + 1; c < DIM; c++)
        if (grid[row][c] >= h) {
            v = false;
            break;
        }
    if (v) return 1;
    v = true;
    for (int r = row - 1; r >= 0; r--)
        if (grid[r][col] >= h) {
            v = false;
            break;
        }
    if (v) return 1;
    v = true;
    for (int r = row + 1; r < DIM; r++)
        if (grid[r][col] >= h) {
            v = false;
            break;
        }
    if (v) return 1;
    return 0;
}

static auto scenic_score(int row, int col) {
    int left = 0;
    for (int c = col - 1; c >= 0; c--) {
        left++;
        if (grid[row][c] >= grid[row][col]) break;
    }
    int right = 0;
    for (int c = col + 1; c < DIM; c++) {
        right++;
        if (grid[row][c] >= grid[row][col]) break;
    }
    int up = 0;
    for (int r = row - 1; r >= 0; r--) {
        up++;
        if (grid[r][col] >= grid[row][col]) break;
    }
    int down = 0;
    for (int r = row + 1; r < DIM; r++) {
        down++;
        if (grid[r][col] >= grid[row][col]) break;
    }
    return left * right * up * down;
}

int main() {
    auto start = high_resolution_clock::now();
    ifstream f("day08.txt");
    for (int row = 0; row < DIM; row++)
        for (int col = 0; col < DIM; col++) f >> grid[row][col];
    int best = 0, visible = (4 * DIM) - 4;
    for (int row = 1; row < DIM - 1; row++)
        for (int col = 1; col < DIM - 1; col++) {
            visible += is_visible(row, col);
            int s = scenic_score(row, col);
            if (s > best) best = s;
        }
    cout << title << endl
         << "Part 1 - " << visible << endl
         << "Part 2 - " << best << endl
         << "Elapsed - "
         << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1e3
         << " ms." << endl;
}
