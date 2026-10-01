// Tiny test helper used by every exercise. Include with: #include "../check.hpp"
#pragma once
#include <cmath>
#include <iostream>

inline int g_pass = 0, g_fail = 0;

#define CHECK(cond)                                                              \
    do {                                                                         \
        if (cond) { ++g_pass; }                                                  \
        else { ++g_fail; std::cerr << "  FAIL line " << __LINE__ << ": " #cond "\n"; } \
    } while (0)

#define CHECK_NEAR(a, b, eps) CHECK(std::fabs((a) - (b)) < (eps))

inline int report() {
    std::cout << (g_fail ? "[FAIL] " : "[ OK ] ") << g_pass << " passed, " << g_fail << " failed\n";
    return g_fail ? 1 : 0;
}
