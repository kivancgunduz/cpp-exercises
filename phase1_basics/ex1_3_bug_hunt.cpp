// Exercise 1.3 — Bug hunt: classic Python-developer mistakes in C++
// This file COMPILES (with warnings — read them!) but every function has a bug.
// Fix each one with the smallest change possible, and be ready to EXPLAIN the bug
// out loud — that's exactly what an interviewer will ask.

#include <numeric>
#include <string>
#include <vector>
#include "../check.hpp"

// Bug 1: should scale every pixel in the caller's vector to [0,1]
void normalize(std::vector<float> pixels) {
    for (size_t i = 0; i < pixels.size(); ++i) pixels[i] /= 255.0f;
}

// Bug 2: should double every element in place
void doubleAll(std::vector<int>& v) {
    for (auto x : v) x *= 2;
}

// Bug 3: should return the average brightness, e.g. {0.5, 0.25} -> 0.375
float average(const std::vector<float>& v) {
    return std::accumulate(v.begin(), v.end(), 0) / static_cast<float>(v.size());
}

// Bug 4: should return a label string
const std::string& makeLabel(int id) {
    std::string s = "obj_" + std::to_string(id);
    return s;
}

// Bug 5: should return the sum of v in reverse order (fine for empty vector too)
int sumReversed(const std::vector<int>& v) {
    int s = 0;
    for (size_t i = v.size() - 1; i >= 0; --i) s += v[i];
    return s;
}

// ---------------- tests (don't edit) ----------------
int main() {
    std::vector<float> px{255.f, 0.f};
    normalize(px);
    CHECK(px[0] == 1.0f);

    std::vector<int> d{1, 2, 3};
    doubleAll(d);
    CHECK(d[2] == 6);

    CHECK(average({0.5f, 0.25f}) == 0.375f);

    std::string label = makeLabel(7);
    CHECK(label == "obj_7");

    CHECK(sumReversed({1, 2, 3}) == 6);
    CHECK(sumReversed({}) == 0);
    return report();
}
