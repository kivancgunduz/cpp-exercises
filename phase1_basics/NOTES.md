# Phase 1 — C++ for a Python developer

## 1. The mental model shift
| Python                              | C++                                                        |
|-------------------------------------|------------------------------------------------------------|
| Interpreted, dynamic types          | Compiled, static types — errors caught at compile time     |
| Every variable is a reference       | Every variable is a VALUE. `b = a` COPIES (vectors too!)   |
| Garbage collector                   | Deterministic lifetime: object dies at end of its scope `}`|
| `int` never overflows               | `int` is 32-bit; overflow of signed int = undefined behaviour |
| `len(v) - 1` can be -1              | `v.size() - 1` on empty vector = 18446744073709551615 (size_t is unsigned!) |
| `a / b` float division              | `7 / 2 == 3` (integer division). Use `7.0 / 2`             |

## 2. Passing arguments — THE most important habit
```cpp
void f(std::vector<float> v);        // copy        — f works on its own copy
void f(const std::vector<float>& v); // read-only   — no copy, can't modify  ← DEFAULT for big objects
void f(std::vector<float>& v);       // in/out      — modifies caller's vector
void f(const std::vector<float>* v); // pointer     — can be nullptr ("optional")
```
Rule of thumb: small types (`int`, `float`, `bool`) → by value. Anything else → `const&`.
Need to modify → `&`. Need a private copy anyway → by value.

## 3. Containers ↔ Python
| Python          | C++                              | Notes                              |
|-----------------|----------------------------------|------------------------------------|
| `list`          | `std::vector<T>`                 | contiguous memory, `push_back`, `reserve` |
| fixed tuple/arr | `std::array<T, N>`               | size known at compile time, stack  |
| `dict`          | `std::unordered_map<K, V>`       | hash map; `std::map` = sorted tree |
| `set`           | `std::unordered_set<T>`          |                                    |
| `str`           | `std::string`                    | mutable!                           |
| `(a, b)`        | `std::pair<A,B>` / `std::tuple`  | `auto [a, b] = p;` unpacks         |
| `deque`         | `std::deque<T>`                  |                                    |
| `heapq`         | `std::priority_queue<T>`         | max-heap by default                |

Gotcha: `m[key]` on an unordered_map INSERTS a default value if key is missing
(like `defaultdict`). Use `m.find(key)` or `m.count(key)` just to check.

## 4. Loops
```cpp
for (size_t i = 0; i < v.size(); ++i) { ... }   // index loop
for (const auto& x : v) { ... }                  // read each element (no copy)
for (auto& x : v) { x *= 2; }                    // modify each element
for (auto x : v) { x *= 2; }                     // ⚠ modifies a COPY — does nothing!
for (const auto& [key, value] : myMap) { ... }   // like dict.items()
```

## 5. Lambdas (≈ Python lambda, but you choose what to capture)
```cpp
int thr = 128;
auto isBright = [thr](int p) { return p > thr; };  // capture thr by value
auto addTo    = [&total](int p) { total += p; };   // capture total by reference
std::sort(v.begin(), v.end(), [](int a, int b) { return a > b; }); // descending
```

## 6. Algorithms (#include <algorithm>, <numeric>)
```cpp
std::sort(v.begin(), v.end());
double s = std::accumulate(v.begin(), v.end(), 0.0);  // ⚠ 0 (int) would truncate floats!
auto it  = std::max_element(v.begin(), v.end());       // returns iterator; *it is the value
std::nth_element(v.begin(), v.begin() + k, v.end());   // O(n) partial sort — perfect for median
std::transform(v.begin(), v.end(), v.begin(), [](float x){ return x * 2; });
```

## 7. Integer types that matter in CV / embedded
`uint8_t` (pixel, 0..255), `int16_t` (gradients), `int32_t`, `float` (32-bit, prefer on embedded),
`double` (64-bit), `size_t` (unsigned, sizes & indices). Include `<cstdint>`.
Arithmetic on `uint8_t` is promoted to `int` first, so `a + b` won't wrap — but storing the result back
into a `uint8_t` will. Always clamp: `std::clamp(val, 0, 255)`.
