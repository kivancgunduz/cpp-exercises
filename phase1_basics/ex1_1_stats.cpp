// Exercise 1.1 — Vector statistics
// Python equivalent: np.mean(v), np.median(v), np.std(v)
//
// Implement the three TODO functions. Rules:
//   * Throw std::invalid_argument if the vector is empty.
//   * stddev = population std-dev (divide by N, like np.std default).
//   * median of even-length vector = average of the two middle values.
// Think about: why is `median` taking its argument BY VALUE while the others take `const&`?
//
// Run:  ./run.sh phase1_basics/ex1_1_stats.cpp

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <vector>
#include "../check.hpp"

double mean(const std::vector<float>& v) {
    // TODO
    if (v.empty()) throw std::invalid_argument("empty vector");
    double total = 0.0;
    for (size_t i = 0; i < v.size(); i++)
    {
        total = total + v[i];
    }
    return total / v.size();
}

double median(std::vector<float> v) {
    // TODO  (bonus: do it in O(n) with std::nth_element instead of std::sort)
    if (v.empty()) throw std::invalid_argument("empty vector");
    size_t n = v.size() / 2;
    nth_element(v.begin(), v.begin() + n, v.end());
    if (v.size() % 2 == 0) {
       float lower = *std::max_element(v.begin(), v.begin() + n);
       return (lower + v[n]) / 2.0;
    }
    return v[n];
}

double stddev(const std::vector<float>& v) {
    // TODO  (you may call mean())
    if (v.empty()) throw std::invalid_argument("empty vector");
    double mean_value = mean(v);
    double total = 0.0;
    for (size_t i = 0; i < v.size(); i++)
    {
        total = total + (v[i] - mean_value) * (v[i] - mean_value);
    }
    double val = total /( v.size());
    return std::sqrt(val);
}

// ---------------- tests (don't edit) ----------------
int main() {
    std::vector<float> a{2, 4, 4, 4, 5, 5, 7, 9};
    CHECK_NEAR(mean(a), 5.0, 1e-9);
    CHECK_NEAR(stddev(a), 2.0, 1e-9);
    CHECK_NEAR(median(a), 4.5, 1e-9);

    std::vector<float> b{9, 1, 5};
    CHECK_NEAR(median(b), 5.0, 1e-9);
    CHECK(b[0] == 9 && b[1] == 1 && b[2] == 5);   // median must NOT reorder the caller's data

    std::vector<float> c{0.5f, 0.25f};
    CHECK_NEAR(mean(c), 0.375, 1e-9);             // catches integer accumulate bug

    bool threw = false;
    try { mean({}); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);

    return report();
}
