// =====================================================================================
//  C++ CHEATSHEET  —  from Python to production C++  (C++17)
// =====================================================================================
//  HOW TO USE
//    ./run.sh phase1_basics/cheatsheat.cpp            run every section demo
//    ./run.sh phase1_basics/cheatsheat.cpp 7          run only section 7
//    ./run.sh phase1_basics/cheatsheat.cpp 7 ex       section 7 + its "YOUR TURN" exercises
//    ./run.sh phase1_basics/cheatsheat.cpp all ex     everything, including all exercises
//    Windows:  run.bat phase1_basics\cheatsheat.cpp 7 ex
//    VS Code:  build with Ctrl+Shift+B, then run   .\cheatsheat.exe 7 ex
//
//  STRUCTURE — every section is a namespace sNN with:
//    * concepts written as real code + comments (Python equivalent on the right)
//    * demo()       runs the examples; CHECK(...) proves each claim, so you can trust it
//    * exercises()  tests for the "YOUR TURN" TODO functions — implement them, run with "ex"
//  Read a section top to bottom, run it, then change things and re-run. Breaking code on
//  purpose and reading the compiler error is one of the fastest ways to learn C++.
//
//  TABLE OF CONTENTS
//    01 Basics: types, variables, const, casts, input/output
//    02 Control flow: if / else, switch, ternary, enum class
//    03 Loops: every kind of loop you will need
//    04 Functions and lambdas
//    05 Strings
//    06 Arrays, pointers and references
//    07 Sequence containers: vector, deque, list            (Python list)
//    08 Associative containers: map, unordered_map, set     (Python dict / set)
//    09 Stack, queue, heap, pair, tuple, optional, variant
//    10 Structs and classes (OOP 1: encapsulation)
//    11 Inheritance and polymorphism (OOP 2)
//    12 Operator overloading
//    13 Memory, RAII, smart pointers, move semantics
//    14 Templates (generic code)
//    15 STL algorithms
//    16 Errors, math and files
//    17 Matrix class and image operations (CV)
// =====================================================================================

#include <algorithm>
#include <array>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <deque>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

#include "../check.hpp"

// NOTE: no `using namespace std;` here (your first version had it). In small scripts it is
// harmless, but in real projects it pulls hundreds of names into scope and causes clashes
// (e.g. your own `count` or `distance` vs std::count / std::distance). Writing std:: is the
// professional habit. Inside a function, `using std::vector;` for one name is fine.

// ---------- small helpers used by all sections --------------------------------------
// Turns any container of printable values into "[a, b, c]" — like Python's print(list).
template <typename Container>
std::string to_str(const Container& c) {
    std::ostringstream os;
    os << '[';
    bool first = true;
    for (const auto& x : c) {
        if (!first) os << ", ";
        os << x;
        first = false;
    }
    os << ']';
    return os.str();
}

void sub(const char* title) { std::cout << "  -- " << title << '\n'; }

// =====================================================================================
// 01  BASICS: types, variables, const, casts, input/output
// =====================================================================================
namespace s01 {

void demo() {
    sub("fundamental types");
    int i = 42;                          // usually 32-bit, range about +-2.1e9
    long long big = 9'000'000'000LL;     // 64-bit. ' is a digit separator (like 9_000_000_000)
    unsigned int u = 7u;                 // can never be negative: wraps modulo 2^32
    float f = 3.14f;                     // 32-bit, ~7 digits. Note the f suffix
    double d = 3.141592653589793;        // 64-bit, ~15 digits  (Python float is a double)
    char c = 'A';                        // 1 byte. 'single quotes' = char, "double" = string
    bool b = true;
    std::string s = "hello";             // std::string is a library class, not a built-in
    std::uint8_t pixel = 255;            // exact-width types from <cstdint>: use for image data
    std::int16_t gradient = -300;        // e.g. Sobel output does not fit in uint8
    std::size_t n = 10;                  // unsigned type for sizes and indices
    std::cout << "    int=" << i << " long long=" << big << " unsigned=" << u << " float=" << f
              << " double=" << d << " char=" << c << " bool=" << std::boolalpha << b
              << std::noboolalpha << " string=" << s << " uint8=" << +pixel  // + prints it as a number
              << " int16=" << gradient << " size_t=" << n << '\n';
    CHECK(sizeof(char) == 1);
    CHECK(sizeof(std::uint8_t) == 1);
    CHECK(sizeof(std::int16_t) == 2);
    CHECK(sizeof(double) == 8);
    std::cout << "    int range: " << std::numeric_limits<int>::min() << " .. "
              << std::numeric_limits<int>::max() << '\n';

    sub("auto: let the compiler deduce the type");
    auto a1 = 5;                         // int
    auto a2 = 5.0;                       // double
    auto a3 = 5.0f;                      // float
    auto a4 = "text";                    // const char*  <- NOT std::string!
    auto a5 = std::string("text");       // std::string
    CHECK((std::is_same_v<decltype(a1), int>));
    CHECK((std::is_same_v<decltype(a2), double>));
    CHECK((std::is_same_v<decltype(a3), float>));
    CHECK((std::is_same_v<decltype(a4), const char*>));
    CHECK((std::is_same_v<decltype(a5), std::string>));

    sub("const and constexpr");
    const double kPi = 3.14159;          // runtime constant: cannot change after init
    constexpr int kWidth = 640;          // compile-time constant
    constexpr int kHeight = 480;
    constexpr int kPixels = kWidth * kHeight;           // computed by the compiler
    static_assert(kPixels == 307200, "checked at compile time, costs nothing at runtime");
    std::array<int, kWidth / 160> small_array{};        // array sizes must be compile-time
    CHECK(small_array.size() == 4);
    CHECK(kPi > 3.0);
    // kPi = 3.0;   // <- uncomment: compile error "assignment of read-only variable"

    sub("integer division, modulo and casts");
    CHECK(7 / 2 == 3);                   // integer division truncates
    CHECK(7 % 2 == 1);
    CHECK(-7 / 2 == -3);                 // truncates toward zero (Python -7 // 2 == -4 !)
    CHECK(7 / 2.0 == 3.5);               // one double operand -> double division
    double avg = static_cast<double>(7) / 2;   // static_cast = the explicit, safe C++ cast
    CHECK(avg == 3.5);
    CHECK(static_cast<int>(3.99) == 3);  // float -> int truncates
    CHECK(std::lround(3.5) == 4);        // round to nearest
    int narrowed = static_cast<int>(1.25 + 1.5);  // your first sum(double,double) returned int:
    CHECK(narrowed == 2);                         // the .75 is silently lost!
    // int y{2.75};   // <- brace init REFUSES narrowing: compile error. Prefer {} init.

    sub("overflow and wrap-around");
    unsigned int zero = 0u;
    unsigned int wrapped = zero - 1u;    // unsigned wraps around
    CHECK(wrapped == std::numeric_limits<unsigned int>::max());
    std::uint8_t p = 250;
    p = static_cast<std::uint8_t>(p + 10);   // 260 mod 256 = 4: the classic pixel bug
    CHECK(p == 4);
    int safe = std::clamp(250 + 10, 0, 255);  // compute in int, then clamp
    CHECK(safe == 255);
    // signed overflow (INT_MAX + 1) is UNDEFINED behaviour — the sanitizer reports it.

    sub("input/output (cin simulated with a string stream so this file runs by itself)");
    std::istringstream fake_cin("42 3.5 hello\nthe whole line\n");
    int in_i = 0;
    double in_d = 0;
    std::string word, line;
    fake_cin >> in_i >> in_d >> word;   // >> reads one token, stops at whitespace
    fake_cin >> std::ws;                // skip the leftover '\n' before getline
    std::getline(fake_cin, line);       // reads a whole line with spaces
    CHECK(in_i == 42 && in_d == 3.5 && word == "hello");
    CHECK(line == "the whole line");
    // In your first version, `cin >> input_string_` only read ONE word of a sentence.
    // Use std::getline(std::cin, input_string_) to read a whole line.
    std::cout << "    fixed 2 decimals: " << std::fixed << std::setprecision(2) << 3.14159
              << " | width 6: [" << std::setw(6) << 42 << "]\n";
    std::cout << std::defaultfloat << std::setprecision(6);  // restore defaults
    // '\n' vs std::endl: endl also flushes the stream -> slow inside loops. Prefer '\n'.
}

// ---- YOUR TURN ----------------------------------------------------------------------
// F = C * 9 / 5 + 32. Careful: what does 9 / 5 evaluate to in C++?
double celsius_to_fahrenheit([[maybe_unused]] double celsius) {
    return 0.0;  // TODO
}
// Luma: gray = 0.299 R + 0.587 G + 0.114 B, rounded to nearest, result in 0..255.
std::uint8_t rgb_to_gray([[maybe_unused]] std::uint8_t r, [[maybe_unused]] std::uint8_t g,
                         [[maybe_unused]] std::uint8_t b) {
    return 0;  // TODO
}

void exercises() {
    CHECK_NEAR(celsius_to_fahrenheit(100.0), 212.0, 1e-9);
    CHECK_NEAR(celsius_to_fahrenheit(37.0), 98.6, 1e-9);
    CHECK(rgb_to_gray(255, 255, 255) == 255);
    CHECK(rgb_to_gray(255, 0, 0) == 76);
    CHECK(rgb_to_gray(10, 200, 30) == 124);
}

}  // namespace s01

// =====================================================================================
// 02  CONTROL FLOW: if / else, switch, ternary, enum class
// =====================================================================================
namespace s02 {

// enum class = scoped enumeration. Values are written Color::Red and do NOT convert to int
// silently (the old C `enum` does). Python equivalent: class Color(Enum).
enum class Color { Red, Green, Blue };

const char* color_name(Color c) {
    switch (c) {                         // switch works on integers, chars and enums
        case Color::Red:   return "red";
        case Color::Green: return "green";
        case Color::Blue:  return "blue";
    }
    return "unknown";                    // -Wall warns if you forget an enum value above
}

std::string brightness_label(int value) {
    if (value < 0 || value > 255) {     // || = or, && = and, ! = not
        return "invalid";
    } else if (value < 85) {             // Python: elif
        return "dark";
    } else if (value < 170) {
        return "medium";
    } else {
        return "bright";
    }
}

int count_vowels_switch(const std::string& text) {
    int vowels = 0;
    for (char ch : text) {
        switch (std::tolower(static_cast<unsigned char>(ch))) {
            case 'a':
            case 'e':                    // several labels share one body ("fallthrough")
            case 'i':
            case 'o':
            case 'u':
                ++vowels;
                break;                   // without break, execution continues into next case!
            default:
                break;
        }
    }
    return vowels;
}

void demo() {
    sub("if / else if / else");
    CHECK(brightness_label(20) == "dark");
    CHECK(brightness_label(128) == "medium");
    CHECK(brightness_label(300) == "invalid");

    sub("switch and enum class");
    Color c = Color::Green;
    std::cout << "    color is " << color_name(c) << '\n';
    CHECK(std::string(color_name(Color::Blue)) == "blue");
    CHECK(count_vowels_switch("Computer Vision") == 6);
    // int x = c;                               // compile error: enum class is not an int
    int as_int = static_cast<int>(Color::Blue);  // explicit conversion is allowed
    CHECK(as_int == 2);

    sub("ternary operator   cond ? a : b      (Python: a if cond else b)");
    int x = 7;
    const char* parity = (x % 2 == 0) ? "even" : "odd";
    CHECK(std::string(parity) == "odd");

    sub("if with initializer (C++17): the variable lives only inside the if/else");
    std::map<std::string, int> stock{{"camera", 3}};
    if (auto it = stock.find("camera"); it != stock.end()) {
        CHECK(it->second == 3);
    } else {
        CHECK(false);
    }

    sub("short-circuit: the right side is evaluated only if needed");
    const int* ptr = nullptr;
    bool safe = (ptr != nullptr) && (*ptr > 0);  // *ptr never runs when ptr is null
    CHECK(!safe);

    sub("truthiness: numbers and pointers convert to bool (0 / nullptr = false)");
    int count = 0;
    if (!count) std::cout << "    count is zero\n";
    // Containers do NOT convert to bool: write `if (!v.empty())`, not `if (v)`.
}

// ---- YOUR TURN ----------------------------------------------------------------------
// From your first cheatsheet: return the larger value, using ONLY the ternary operator.
int get_max_value([[maybe_unused]] int x, [[maybe_unused]] int y) {
    return 0;  // TODO
}
// Score -> grade with an if / else-if ladder: >=90 "A", >=80 "B", >=70 "C", else "F".
// Bonus: anything outside 0..100 returns "invalid".
std::string grade([[maybe_unused]] int score) {
    return "";  // TODO
}

void exercises() {
    CHECK(get_max_value(3, 9) == 9);
    CHECK(get_max_value(-1, -5) == -1);
    CHECK(grade(95) == "A");
    CHECK(grade(80) == "B");
    CHECK(grade(79) == "C");
    CHECK(grade(10) == "F");
    CHECK(grade(101) == "invalid");
}

}  // namespace s02

// =====================================================================================
// 03  LOOPS: every kind of loop you will need
// =====================================================================================
namespace s03 {

void demo() {
    std::vector<int> v{10, 20, 30, 40, 50};

    sub("1. classic index loop                 for i in range(len(v))");
    int sum = 0;
    for (std::size_t i = 0; i < v.size(); ++i) {  // ++i: prefer pre-increment
        sum += v[i];
    }
    CHECK(sum == 150);

    sub("2. start, stop, step                  for i in range(0, 10, 3)");
    std::vector<int> stepped;
    for (int i = 0; i < 10; i += 3) stepped.push_back(i);
    CHECK(stepped == std::vector<int>({0, 3, 6, 9}));

    sub("3. counting down SAFELY with size_t   for i in reversed(range(len(v)))");
    std::vector<int> backwards;
    for (std::size_t i = v.size(); i-- > 0;) {     // test, THEN decrement: never wraps
        backwards.push_back(v[i]);
    }
    CHECK(backwards == std::vector<int>({50, 40, 30, 20, 10}));
    // for (std::size_t i = v.size() - 1; i >= 0; --i)   <- infinite loop: size_t is never < 0

    sub("4. range-for: const& reads, & modifies, plain value copies");
    long total = 0;
    for (const int& x : v) total += x;   // read only, no copy (matters for big objects)
    for (int& x : v) x += 1;             // modifies the vector in place
    // for (int x : v) x = 0;            // modifies a COPY: v unchanged (the compiler warns)
    CHECK(total == 150);
    CHECK(v.front() == 11);

    sub("5. two loop variables: reverse in place");
    std::vector<int> r{1, 2, 3, 4, 5};
    for (std::size_t lo = 0, hi = r.size() - 1; lo < hi; ++lo, --hi) {
        std::swap(r[lo], r[hi]);
    }
    CHECK(r == std::vector<int>({5, 4, 3, 2, 1}));

    sub("6. while                              while n != 1:");
    int n = 27, steps = 0;
    while (n != 1) {                     // Collatz sequence
        n = (n % 2 == 0) ? n / 2 : 3 * n + 1;
        ++steps;
    }
    CHECK(steps == 111);

    sub("7. do-while: the body always runs at least once");
    int attempts = 0;
    do {
        ++attempts;                      // e.g. "grab a frame, retry while it failed"
    } while (attempts < 0);
    CHECK(attempts == 1);

    sub("8. break and continue");
    std::vector<int> odds_until_7;
    for (int i = 0; i < 100; ++i) {
        if (i % 2 == 0) continue;        // skip to next iteration
        if (i > 7) break;                // leave the loop
        odds_until_7.push_back(i);
    }
    CHECK(odds_until_7 == std::vector<int>({1, 3, 5, 7}));

    sub("9. nested loops over a 2D grid stored row-major in one vector");
    const int rows = 3, cols = 4;
    std::vector<int> grid(rows * cols);
    for (int y = 0; y < rows; ++y) {             // outer loop = rows
        for (int x = 0; x < cols; ++x) {         // inner loop = columns: memory order, cache friendly
            grid[y * cols + x] = y * 10 + x;     // index = row * width + col
        }
    }
    CHECK(grid[2 * cols + 3] == 23);

    sub("10. leave nested loops: wrap them in a lambda and return (no goto needed)");
    auto find_first = [&](int target) -> std::pair<int, int> {
        for (int y = 0; y < rows; ++y)
            for (int x = 0; x < cols; ++x)
                if (grid[y * cols + x] == target) return {y, x};
        return {-1, -1};
    };
    CHECK(find_first(12) == std::make_pair(1, 2));
    CHECK(find_first(99) == std::make_pair(-1, -1));

    sub("11. explicit iterators and reverse iterators");
    std::vector<std::string> names{"ada", "linus", "grace"};
    std::string joined;
    for (auto it = names.begin(); it != names.end(); ++it) joined += *it + " ";
    CHECK(joined == "ada linus grace ");
    std::string rjoined;
    for (auto it = names.rbegin(); it != names.rend(); ++it) rjoined += *it + " ";
    CHECK(rjoined == "grace linus ada ");

    sub("12. enumerate: index and value        for i, name in enumerate(names)");
    for (std::size_t i = 0; i < names.size(); ++i) {
        std::cout << "    " << i << ": " << names[i] << '\n';
    }

    sub("13. dict.items(): structured bindings over a map");
    std::map<std::string, int> fps{{"thermal", 30}, {"visible", 60}};
    for (const auto& [sensor, rate] : fps) {
        std::cout << "    " << sensor << " -> " << rate << " fps\n";
    }

    sub("14. characters of a string");
    int upper = 0;
    for (char ch : std::string("Hello World")) {
        if (std::isupper(static_cast<unsigned char>(ch))) ++upper;
    }
    CHECK(upper == 2);

    sub("15. std::for_each with a lambda");
    int squares = 0;
    std::for_each(v.begin(), v.end(), [&squares](int x) { squares += x * x; });
    CHECK(squares > 0);

    sub("16. zip two vectors                   for a, b in zip(xs, ys)");
    std::vector<int> xs{1, 2, 3}, ys{4, 5, 6};
    int dot = 0;
    for (std::size_t i = 0; i < std::min(xs.size(), ys.size()); ++i) dot += xs[i] * ys[i];
    CHECK(dot == 32);

    sub("17. infinite loop with an exit condition   while True:");
    int ticks = 0;
    for (;;) {                           // same as while (true)
        if (++ticks == 5) break;
    }
    CHECK(ticks == 5);
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Sum of all even numbers from 1 to n inclusive (use a for loop with a step).
int sum_even([[maybe_unused]] int n) { return 0; }  // TODO
// Reverse a string using a loop (no std::reverse).
std::string reverse_string([[maybe_unused]] const std::string& s) { return ""; }  // TODO
// n x n multiplication table: table[i][j] = (i + 1) * (j + 1). Use nested loops.
std::vector<std::vector<int>> multiplication_table([[maybe_unused]] int n) { return {}; }  // TODO
// Number of decimal digits of a non-negative number, with a while loop (0 has 1 digit).
int count_digits([[maybe_unused]] int n) { return 0; }  // TODO

void exercises() {
    CHECK(sum_even(10) == 30);
    CHECK(sum_even(1) == 0);
    CHECK(reverse_string("pixel") == "lexip");
    CHECK(reverse_string("").empty());
    auto t = multiplication_table(3);
    CHECK(t.size() == 3 && t[2].size() == 3 && t[2][2] == 9 && t[0][1] == 2);
    CHECK(count_digits(0) == 1);
    CHECK(count_digits(12345) == 5);
}

}  // namespace s03

// =====================================================================================
// 04  FUNCTIONS AND LAMBDAS
// =====================================================================================
namespace s04 {

// Passing arguments — the single most important habit coming from Python:
void add_one_copy(int x) { x += 1; }           // gets a copy: caller unchanged
void add_one_ref(int& x) { x += 1; }           // reference: changes the caller's variable
double mean(const std::vector<double>& v) {    // const&: read a big object without copying it
    if (v.empty()) throw std::invalid_argument("mean of empty vector");
    return std::accumulate(v.begin(), v.end(), 0.0) / static_cast<double>(v.size());
}
bool safe_divide(double a, double b, double* out) {  // pointer: optional output, C style
    if (b == 0.0 || out == nullptr) return false;
    *out = a / b;
    return true;
}

// Default arguments (must be the last parameters)
double rect_area(double width, double height = 1.0) { return width * height; }

// Overloading: same name, different parameter types. The compiler picks the best match.
int twice(int x) { return 2 * x; }
double twice(double x) { return 2.0 * x; }
std::string twice(const std::string& s) { return s + s; }

// Returning several values: prefer a small struct (named fields), else pair / tuple.
struct MinMax {
    int min;
    int max;
};
MinMax min_max(const std::vector<int>& v) {
    if (v.empty()) throw std::invalid_argument("min_max of empty vector");
    MinMax r{v[0], v[0]};
    for (int x : v) {
        r.min = std::min(r.min, x);
        r.max = std::max(r.max, x);
    }
    return r;
}
std::pair<int, int> div_mod(int a, int b) { return {a / b, a % b}; }   // Python: divmod
std::tuple<std::string, int, double> parse_record() { return {"cam0", 640, 29.97}; }

// Recursion
unsigned long long factorial(unsigned n) { return n <= 1 ? 1ULL : n * factorial(n - 1); }

// A static local variable keeps its value between calls (initialised once).
int next_id() {
    static int counter = 0;
    return ++counter;
}

// Functions as arguments: function pointer (C style) and std::function (any callable).
int apply_twice(int (*f)(int), int x) { return f(f(x)); }
int square(int x) { return x * x; }
int apply(const std::function<int(int)>& f, int x) { return f(x); }

// A function that returns a function (a closure) — like a Python closure.
std::function<int(int)> make_multiplier(int factor) {
    return [factor](int x) { return x * factor; };  // factor is captured BY VALUE: safe
}

void demo() {
    sub("by value vs by reference vs pointer");
    int a = 5;
    add_one_copy(a);
    CHECK(a == 5);
    add_one_ref(a);
    CHECK(a == 6);
    double q = 0;
    CHECK(safe_divide(1.0, 4.0, &q) && q == 0.25);
    CHECK(!safe_divide(1.0, 0.0, &q));
    CHECK(mean({1.0, 2.0, 3.0}) == 2.0);

    sub("default arguments and overloading");
    CHECK(rect_area(3.0) == 3.0);
    CHECK(rect_area(3.0, 2.0) == 6.0);
    CHECK(twice(4) == 8);
    CHECK(twice(1.5) == 3.0);
    CHECK(twice(std::string("ab")) == "abab");

    sub("multiple return values");
    auto [lo, hi] = min_max({4, -2, 9});     // structured binding unpacks the struct
    CHECK(lo == -2 && hi == 9);
    auto [quot, rem] = div_mod(17, 5);
    CHECK(quot == 3 && rem == 2);
    auto [name, width, rate] = parse_record();
    std::cout << "    " << name << " " << width << "px " << rate << " fps\n";

    sub("recursion and static locals");
    CHECK(factorial(10) == 3628800ULL);
    int first = next_id();
    int second = next_id();
    CHECK(second == first + 1);

    sub("lambdas: [capture](params) { body }        Python: lambda x: x * 2");
    auto add = [](int x, int y) { return x + y; };
    CHECK(add(2, 3) == 5);
    int threshold = 100;
    auto is_bright = [threshold](int p) { return p > threshold; };   // capture one by value
    CHECK(is_bright(200) && !is_bright(50));
    int hits = 0;
    auto count_hit = [&hits]() { ++hits; };                          // capture by reference
    count_hit();
    count_hit();
    CHECK(hits == 2);
    // [=] captures everything used by value, [&] everything by reference.
    // DANGER: a lambda capturing a local by & must not outlive that local.
    auto counter = [n = 0]() mutable { return ++n; };   // own state; mutable lets it change
    counter();
    CHECK(counter() == 2);
    auto generic = [](const auto& x) { return x + x; };  // generic lambda (template)
    CHECK(generic(3) == 6 && generic(std::string("hi")) == "hihi");
    auto lambda_type = [](double x) -> int { return static_cast<int>(x); };  // explicit return
    CHECK(lambda_type(2.9) == 2);

    sub("functions as values");
    CHECK(apply_twice(square, 3) == 81);
    CHECK(apply([](int x) { return x - 1; }, 10) == 9);
    auto triple = make_multiplier(3);
    CHECK(triple(7) == 21);
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Iterative Fibonacci: fib(0)=0, fib(1)=1, fib(10)=55. Use a loop, not recursion.
long long fibonacci([[maybe_unused]] int n) { return 0; }  // TODO
// Greatest common divisor with RECURSION (Euclid): gcd(a, 0) = a; gcd(a, b) = gcd(b, a % b).
int gcd([[maybe_unused]] int a, [[maybe_unused]] int b) { return 0; }  // TODO
// Return a lambda that clamps its int argument into [lo, hi].
std::function<int(int)> make_clamper([[maybe_unused]] int lo, [[maybe_unused]] int hi) {
    return [](int x) { return x; };  // TODO
}

void exercises() {
    CHECK(fibonacci(0) == 0);
    CHECK(fibonacci(10) == 55);
    CHECK(fibonacci(50) == 12586269025LL);
    CHECK(gcd(48, 18) == 6);
    CHECK(gcd(7, 13) == 1);
    auto clamp_u8 = make_clamper(0, 255);
    CHECK(clamp_u8(300) == 255 && clamp_u8(-4) == 0 && clamp_u8(17) == 17);
}

}  // namespace s04

// =====================================================================================
// 05  STRINGS
// =====================================================================================
namespace s05 {

// Python: "a,b,c".split(",")
std::vector<std::string> split(const std::string& text, char delim) {
    std::vector<std::string> parts;
    std::string part;
    std::istringstream stream(text);
    while (std::getline(stream, part, delim)) parts.push_back(part);
    return parts;
}

// Python: ", ".join(parts)
std::string join(const std::vector<std::string>& parts, const std::string& sep) {
    std::string out;
    for (std::size_t i = 0; i < parts.size(); ++i) {
        if (i > 0) out += sep;
        out += parts[i];
    }
    return out;
}

// Python: s.upper()   — note the unsigned char cast: std::toupper(negative char) is UB
std::string to_upper(std::string s) {           // by value: we need a copy to modify anyway
    for (char& ch : s) ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
    return s;
}

// Python: s.strip()
std::string trim(const std::string& s) {
    const char* ws = " \t\n\r";
    const auto start = s.find_first_not_of(ws);
    if (start == std::string::npos) return "";
    const auto end = s.find_last_not_of(ws);
    return s.substr(start, end - start + 1);
}

// string_view: a cheap read-only "window" onto characters (pointer + length, no copy).
bool starts_with(std::string_view text, std::string_view prefix) {
    return text.substr(0, prefix.size()) == prefix;
}

// Your first get_string_details() copied a string into `new char[len]` and then wrote
// arr[len] = '\0'  ->  one byte PAST the end of the allocation (heap buffer overflow; the
// sanitizer reports it). It needed new char[len + 1]. Better: never use new[] for this —
// std::string and std::vector own their memory and free it automatically.
std::vector<char> to_chars(const std::string& s) { return std::vector<char>(s.begin(), s.end()); }

void demo() {
    sub("basic operations (the GeeksforGeeks string functions)");
    std::string s = "Computer";
    CHECK(s.length() == 8 && s.size() == 8);   // length() == size()
    CHECK(s[0] == 'C' && s.back() == 'r');
    CHECK(s.substr(0, 4) == "Comp");          // substr(pos, count)   Python: s[0:4]
    CHECK(s.substr(3) == "puter");            //                      Python: s[3:]
    s.append(" Vision");                      // or s += " Vision"
    CHECK(s == "Computer Vision");
    CHECK(s.compare("Computer Vision") == 0); // 0 = equal, <0 / >0 = ordering. == is simpler.
    CHECK(std::string("abc") < std::string("abd"));
    CHECK(!s.empty());

    sub("search: find returns an index, or npos when not found");
    CHECK(s.find("Vision") == 9);
    CHECK(s.find("xyz") == std::string::npos);  // Python's -1 / `in` check
    CHECK(s.rfind('o') == 13);                  // search from the right
    bool contains = s.find("put") != std::string::npos;   // Python: "put" in s
    CHECK(contains);

    sub("modify: insert, erase, replace");
    std::string m = "hello world";
    m.insert(0, ">> ");
    CHECK(m == ">> hello world");
    m.erase(0, 3);                            // erase(pos, count)
    CHECK(m == "hello world");
    m.replace(m.find("world"), 5, "pixels");  // replace(pos, count, new)
    CHECK(m == "hello pixels");

    sub("numbers <-> strings");
    CHECK(std::to_string(42) == "42");
    CHECK(std::stoi("123") == 123);           // throws std::invalid_argument on "abc"
    CHECK(std::stod("2.5") == 2.5);
    std::ostringstream os;                    // the f-string / format replacement
    os << "frame_" << std::setw(4) << std::setfill('0') << 7 << ".png";
    CHECK(os.str() == "frame_0007.png");

    sub("split, join, upper, trim, string_view");
    auto parts = split("x,y,w,h", ',');
    CHECK(parts.size() == 4 && parts[2] == "w");
    CHECK(join(parts, "|") == "x|y|w|h");
    CHECK(to_upper("rgb") == "RGB");
    CHECK(trim("  padded \n") == "padded");
    CHECK(starts_with("thermal_001.raw", "thermal"));

    sub("characters: <cctype>");
    CHECK(std::isdigit('7') && std::isalpha('a') && std::isspace(' '));
    CHECK('a' + 1 == 'b');                     // chars are small integers
    CHECK(static_cast<int>('A') == 65);

    sub("your get_string_details, fixed");
    auto chars = to_chars("abc");
    CHECK(chars.size() == 3 && chars[2] == 'c');
}

// ---- YOUR TURN ----------------------------------------------------------------------
// True if s reads the same backwards, ignoring case and non-letters. "A man, a plan..." -> true
bool is_palindrome([[maybe_unused]] const std::string& s) { return false; }  // TODO
// Count words separated by any amount of whitespace. Hint: std::istringstream and >>.
int count_words([[maybe_unused]] const std::string& s) { return 0; }  // TODO
// "frame", 42 -> "frame_000042.png" (index padded to 6 digits)
std::string frame_filename([[maybe_unused]] const std::string& prefix, [[maybe_unused]] int index) {
    return "";  // TODO
}

void exercises() {
    CHECK(is_palindrome("A man, a plan, a canal: Panama"));
    CHECK(!is_palindrome("pixel"));
    CHECK(count_words("  detect   track identify ") == 3);
    CHECK(count_words("") == 0);
    CHECK(frame_filename("frame", 42) == "frame_000042.png");
}

}  // namespace s05

// =====================================================================================
// 06  ARRAYS, POINTERS AND REFERENCES
// =====================================================================================
namespace s06 {

// A C array passed to a function DECAYS to a pointer: the size is lost, so pass it too.
int sum_c_array(const int* data, std::size_t count) {
    int total = 0;
    for (std::size_t i = 0; i < count; ++i) total += data[i];
    return total;
}

// std::array keeps its size in the type: no decay, no separate size argument.
int sum_std_array(const std::array<int, 5>& a) { return std::accumulate(a.begin(), a.end(), 0); }

void demo() {
    sub("C-style arrays (fixed size, no bounds checking)");
    int c_arr[5] = {1, 2, 3, 4, 5};
    int zeros[4] = {};                        // all zero
    int grid[2][3] = {{1, 2, 3}, {4, 5, 6}};  // multi-dimensional array
    CHECK(std::size(c_arr) == 5);
    CHECK(zeros[3] == 0);
    CHECK(grid[1][2] == 6);
    CHECK(sum_c_array(c_arr, std::size(c_arr)) == 15);
    // c_arr[5] = 0;   // out of bounds: undefined behaviour (the sanitizer catches it)

    sub("std::array: fixed size, knows its size, can be copied and compared");
    std::array<int, 5> a{5, 4, 3, 2, 1};
    std::array<int, 5> b = a;                 // real copy (a C array cannot be assigned)
    CHECK(a == b);
    CHECK(sum_std_array(a) == 15);
    a.fill(0);
    CHECK(a[0] == 0 && b[0] == 5);
    bool threw = false;
    try { (void)a.at(10); } catch (const std::out_of_range&) { threw = true; }  // .at() checks
    CHECK(threw);
    std::array<std::array<int, 3>, 2> grid2{{{1, 2, 3}, {4, 5, 6}}};
    CHECK(grid2[1][0] == 4);

    sub("references: another name for an existing variable");
    int x = 10;
    int& rx = x;                              // must be initialised, cannot be re-seated
    rx = 20;
    CHECK(x == 20);
    const int& crx = x;                       // read-only alias
    CHECK(crx == 20);

    sub("pointers: a variable that holds an address");
    int* px = &x;                             // & = "address of"
    *px = 30;                                 // * = "the value at that address"
    CHECK(x == 30);
    int* nothing = nullptr;                   // points nowhere (Python: None)
    if (nothing == nullptr) std::cout << "    null pointer: never dereference it\n";
    int y = 1;
    px = &y;                                  // unlike a reference, a pointer can be re-pointed
    CHECK(*px == 1);

    sub("pointer arithmetic over contiguous memory (how image buffers are walked)");
    std::vector<std::uint8_t> image{10, 20, 30, 40, 50, 60};  // 2 rows x 3 cols
    const int width = 3;
    const std::uint8_t* row1 = image.data() + 1 * width;       // pointer to start of row 1
    CHECK(row1[0] == 40 && *(row1 + 2) == 60);
    std::ptrdiff_t distance = row1 - image.data();
    CHECK(distance == 3);

    sub("const and pointers (read right to left)");
    const int* ptr_to_const = &x;     // cannot change *ptr_to_const, can re-point it
    int* const const_ptr = &x;        // can change *const_ptr, cannot re-point it
    *const_ptr = 31;
    CHECK(*ptr_to_const == 31);

    sub("dynamic memory: new / delete exist, but you should almost never need them");
    int* raw = new int[3]{1, 2, 3};   // manual: YOU must delete[] it, exactly once
    CHECK(raw[2] == 3);
    delete[] raw;                     // forget this = leak; do it twice = crash
    std::vector<int> owned{1, 2, 3};  // same thing, freed automatically (see section 13)
    CHECK(owned[2] == 3);
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Swap two ints through POINTERS.
void swap_ptr([[maybe_unused]] int* a, [[maybe_unused]] int* b) {}  // TODO
// Swap two ints through REFERENCES. Which version is nicer to call?
void swap_ref([[maybe_unused]] int& a, [[maybe_unused]] int& b) {}  // TODO
// Sum of the main diagonal of a 3x3 std::array matrix.
int trace3([[maybe_unused]] const std::array<std::array<int, 3>, 3>& m) { return 0; }  // TODO

void exercises() {
    int a = 1, b = 2;
    swap_ptr(&a, &b);
    CHECK(a == 2 && b == 1);
    swap_ref(a, b);
    CHECK(a == 1 && b == 2);
    std::array<std::array<int, 3>, 3> m{{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    CHECK(trace3(m) == 15);
}

}  // namespace s06

// =====================================================================================
// 07  SEQUENCE CONTAINERS: vector, deque, list            (Python list)
// =====================================================================================
namespace s07 {

void demo() {
    sub("std::vector: THE default container (contiguous, fast, growable)");
    std::vector<int> v;                       // []
    v.push_back(3);                           // append
    v.emplace_back(4);                        // append, constructing in place
    v.insert(v.begin(), 1);                   // insert at index 0
    v.insert(v.begin() + 1, 2);               // insert at index 1
    CHECK(v == std::vector<int>({1, 2, 3, 4}));
    CHECK(v.front() == 1 && v.back() == 4 && v[2] == 3 && v.at(2) == 3);
    v.pop_back();                             // remove last (returns nothing, unlike Python)
    v.erase(v.begin());                       // del v[0]
    CHECK(v == std::vector<int>({2, 3}));

    sub("construction shortcuts");
    std::vector<int> filled(5, 7);            // [7] * 5
    std::vector<int> listed{1, 2, 3};         // [1, 2, 3]
    std::vector<int> counting(5);             // [0] * 5
    std::iota(counting.begin(), counting.end(), 0);  // list(range(5))
    CHECK(filled.size() == 5 && filled[4] == 7);
    CHECK(counting == std::vector<int>({0, 1, 2, 3, 4}));

    sub("size vs capacity: reserve() avoids repeated reallocations");
    std::vector<int> big;
    big.reserve(1000);                        // allocate once
    for (int i = 0; i < 1000; ++i) big.push_back(i);
    CHECK(big.size() == 1000 && big.capacity() >= 1000);
    big.clear();                              // size 0, capacity kept
    CHECK(big.empty());

    sub("slicing, concatenating, searching");
    std::vector<int> nums{5, 1, 4, 1, 5, 9, 2, 6};
    std::vector<int> slice(nums.begin() + 1, nums.begin() + 4);   // nums[1:4]
    CHECK(slice == std::vector<int>({1, 4, 1}));
    std::vector<int> joined = slice;
    joined.insert(joined.end(), {7, 8});                          // joined += [7, 8]
    CHECK(joined.size() == 5);
    auto it = std::find(nums.begin(), nums.end(), 9);             // 9 in nums ?
    CHECK(it != nums.end() && std::distance(nums.begin(), it) == 5);  // nums.index(9)
    CHECK(std::count(nums.begin(), nums.end(), 1) == 2);          // nums.count(1)

    sub("sort, reverse, and the erase-remove idiom");
    std::sort(nums.begin(), nums.end());                          // nums.sort()
    CHECK(nums.front() == 1 && nums.back() == 9);
    std::sort(nums.begin(), nums.end(), std::greater<int>());     // reverse=True
    std::reverse(nums.begin(), nums.end());
    // remove every 1: std::remove shifts the kept values left; erase cuts off the tail
    nums.erase(std::remove(nums.begin(), nums.end(), 1), nums.end());
    CHECK(std::count(nums.begin(), nums.end(), 1) == 0);
    // remove by condition (Python: [x for x in nums if x <= 5])
    nums.erase(std::remove_if(nums.begin(), nums.end(), [](int x) { return x > 5; }), nums.end());
    CHECK(std::all_of(nums.begin(), nums.end(), [](int x) { return x <= 5; }));

    sub("2D vector (simple, but rows are separate allocations: images use one buffer)");
    const std::size_t rows = 2, cols = 3;
    std::vector<std::vector<int>> grid(rows, std::vector<int>(cols, 0));
    grid[1][2] = 9;
    CHECK(grid.size() == 2 && grid[0].size() == 3 && grid[1][2] == 9);

    sub("iterator invalidation: the #1 vector bug");
    // for (auto& x : v) v.push_back(x);   // push_back may reallocate -> x dangles -> crash
    // Rule: don't add/remove elements while iterating over the same vector.

    sub("std::deque: fast insert/remove at BOTH ends   (collections.deque)");
    std::deque<int> dq{2, 3};
    dq.push_front(1);
    dq.push_back(4);
    dq.pop_front();
    CHECK(dq.front() == 2 && dq.back() == 4 && dq.size() == 3);

    sub("std::list: doubly linked list. O(1) insert in the middle, but no [] and slow to scan");
    std::list<std::string> tasks{"capture", "infer"};
    auto pos = std::next(tasks.begin());      // iterator to "infer"
    tasks.insert(pos, "preprocess");          // insert before it, no shifting
    CHECK(to_str(tasks) == "[capture, preprocess, infer]");
    // In practice: use vector unless you have measured a reason not to.
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Remove duplicates but keep the FIRST occurrence order: {3,1,3,2,1} -> {3,1,2}.
// Hint: an std::unordered_set<int> of seen values.
std::vector<int> unique_keep_order([[maybe_unused]] const std::vector<int>& v) { return {}; }  // TODO
// Rotate left by k: {1,2,3,4,5}, k=2 -> {3,4,5,1,2}. Works for k > size too. (std::rotate exists — try both)
std::vector<int> rotate_left([[maybe_unused]] std::vector<int> v, [[maybe_unused]] std::size_t k) { return {}; }  // TODO
// Flatten a 2D vector row by row into one vector.
std::vector<int> flatten([[maybe_unused]] const std::vector<std::vector<int>>& grid) { return {}; }  // TODO

void exercises() {
    CHECK(unique_keep_order({3, 1, 3, 2, 1}) == std::vector<int>({3, 1, 2}));
    CHECK(rotate_left({1, 2, 3, 4, 5}, 2) == std::vector<int>({3, 4, 5, 1, 2}));
    CHECK(rotate_left({1, 2, 3}, 4) == std::vector<int>({2, 3, 1}));
    CHECK(flatten({{1, 2}, {3}, {}, {4, 5}}) == std::vector<int>({1, 2, 3, 4, 5}));
}

}  // namespace s07

// =====================================================================================
// 08  ASSOCIATIVE CONTAINERS: map, unordered_map, set     (Python dict / set)
// =====================================================================================
namespace s08 {

// Python: d.get(key, default). C++ has no built-in for it, so it is a common helper.
template <typename Map, typename Key, typename Value>
Value get_or(const Map& m, const Key& key, const Value& fallback) {
    auto it = m.find(key);
    return it != m.end() ? it->second : fallback;
}

void demo() {
    sub("std::unordered_map = hash table = Python dict (no ordering)");
    std::unordered_map<std::string, int> ages{{"alice", 30}, {"bob", 25}};
    ages["carol"] = 35;                       // d[k] = v  (insert or overwrite)
    ages.insert({"bob", 99});                 // insert does NOT overwrite an existing key
    CHECK(ages["bob"] == 25);
    ages.insert_or_assign("bob", 26);         // explicit overwrite
    CHECK(ages.at("bob") == 26);              // at(): throws std::out_of_range if missing (KeyError)
    ages.try_emplace("dave", 40);             // insert only if absent (dict.setdefault)
    CHECK(ages.size() == 4);

    sub("THE dict trap: operator[] inserts a default value for a missing key");
    std::size_t before = ages.size();
    int ghost = ages["zoe"];                  // reading creates "zoe" -> 0 !
    CHECK(ghost == 0 && ages.size() == before + 1);
    CHECK(ages.erase("zoe") == 1);            // del d["zoe"] (returns how many were removed)

    sub("membership and get-with-default");
    CHECK(ages.count("alice") == 1);          // "alice" in d    (C++20 adds .contains())
    CHECK(ages.find("nobody") == ages.end());
    CHECK(get_or(ages, std::string("nobody"), -1) == -1);

    sub("iterate: for key, value in d.items()   (unordered_map order is arbitrary!)");
    int total_age = 0;
    for (const auto& [name, age] : ages) total_age += age;
    CHECK(total_age == 30 + 26 + 35 + 40);

    sub("counting (collections.Counter)");
    std::unordered_map<char, int> freq;
    for (char ch : std::string("tracking")) ++freq[ch];   // here the auto-insert is useful
    CHECK(freq['k'] == 1 && freq['n'] == 1 && freq['a'] == 1 && freq.at('t') == 1);

    sub("std::map = SORTED by key (red-black tree, O(log n))");
    std::map<int, std::string> by_id{{30, "c"}, {10, "a"}, {20, "b"}};
    CHECK(by_id.begin()->first == 10);        // smallest key
    CHECK(by_id.rbegin()->first == 30);       // largest key
    auto lb = by_id.lower_bound(15);          // first key >= 15
    CHECK(lb->first == 20);
    std::string ordered;
    for (const auto& [id, label] : by_id) ordered += label;
    CHECK(ordered == "abc");

    sub("sort a dict by value: copy into a vector of pairs, then sort");
    std::vector<std::pair<std::string, int>> items(ages.begin(), ages.end());
    std::sort(items.begin(), items.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    CHECK(items.front().first == "dave");

    sub("nested: dict of lists");
    std::map<std::string, std::vector<std::string>> detections;
    detections["frame_1"].push_back("drone");  // [] creates the empty vector first
    detections["frame_1"].push_back("bird");
    CHECK(detections["frame_1"].size() == 2);

    sub("std::set (sorted, unique) and std::unordered_set (hash, unique)");
    std::set<int> s{5, 1, 3, 3, 1};
    CHECK(s.size() == 3 && *s.begin() == 1);  // duplicates removed, sorted
    s.insert(2);
    s.erase(5);
    CHECK(s.count(2) == 1 && s.count(5) == 0);
    std::unordered_set<std::string> seen{"a", "b"};
    bool inserted = seen.insert("a").second;  // .second = was it new?
    CHECK(!inserted);

    sub("set algebra on sorted sets: |  &  -");
    std::set<int> a{1, 2, 3, 4}, b{3, 4, 5};
    std::set<int> uni, inter, diff;
    std::set_union(a.begin(), a.end(), b.begin(), b.end(), std::inserter(uni, uni.begin()));
    std::set_intersection(a.begin(), a.end(), b.begin(), b.end(), std::inserter(inter, inter.begin()));
    std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::inserter(diff, diff.begin()));
    CHECK(to_str(uni) == "[1, 2, 3, 4, 5]");
    CHECK(to_str(inter) == "[3, 4]");
    CHECK(to_str(diff) == "[1, 2]");

    // Which one? unordered_map/set: O(1) average, use by default.
    //            map/set: when you need sorted order, ranges (lower_bound) or min/max keys.
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Two-sum: indices (i, j), i < j, with v[i] + v[j] == target, in ONE pass using an
// unordered_map<value, index>. Return {-1, -1} if none.
std::pair<int, int> two_sum([[maybe_unused]] const std::vector<int>& v, [[maybe_unused]] int target) {
    return {-1, -1};  // TODO
}
// Group words by first letter: {"bird","drone","blob"} -> {'b': {"bird","blob"}, 'd': {"drone"}}
std::map<char, std::vector<std::string>> group_by_first_letter(
    [[maybe_unused]] const std::vector<std::string>& words) {
    return {};  // TODO
}

void exercises() {
    CHECK(two_sum({2, 7, 11, 15}, 9) == std::make_pair(0, 1));
    CHECK(two_sum({3, 2, 4}, 6) == std::make_pair(1, 2));
    CHECK(two_sum({1, 2}, 10) == std::make_pair(-1, -1));
    auto g = group_by_first_letter({"bird", "drone", "blob"});
    CHECK(g.size() == 2);
    CHECK(g.count('b') && g['b'] == std::vector<std::string>({"bird", "blob"}));
}

}  // namespace s08

// =====================================================================================
// 09  STACK, QUEUE, HEAP, PAIR, TUPLE, OPTIONAL, VARIANT
// =====================================================================================
namespace s09 {

struct Detection {
    std::string label;
    float score;
};

// std::optional<T> = "a T or nothing" (Python: return None). Better than -1 magic values.
std::optional<std::size_t> index_of(const std::vector<int>& v, int target) {
    for (std::size_t i = 0; i < v.size(); ++i)
        if (v[i] == target) return i;
    return std::nullopt;
}

void demo() {
    sub("std::stack (LIFO)                       Python: list.append / list.pop");
    std::stack<int> st;
    st.push(1);
    st.push(2);
    st.push(3);
    CHECK(st.top() == 3);                     // look at the top
    st.pop();                                 // remove it (returns void!)
    CHECK(st.top() == 2 && st.size() == 2);

    sub("std::queue (FIFO)                       Python: deque.append / deque.popleft");
    std::queue<std::string> frames;
    frames.push("f0");
    frames.push("f1");
    CHECK(frames.front() == "f0" && frames.back() == "f1");
    frames.pop();
    CHECK(frames.front() == "f1");

    sub("std::priority_queue (heap)              Python: heapq (which is a MIN-heap)");
    std::priority_queue<int> max_heap;        // C++ default: MAX-heap
    for (int x : {5, 1, 8, 3}) max_heap.push(x);
    CHECK(max_heap.top() == 8);
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;  // min-heap
    for (int x : {5, 1, 8, 3}) min_heap.push(x);
    CHECK(min_heap.top() == 1);
    auto by_score = [](const Detection& a, const Detection& b) { return a.score < b.score; };
    std::priority_queue<Detection, std::vector<Detection>, decltype(by_score)> best(by_score);
    best.push({"bird", 0.40f});
    best.push({"drone", 0.92f});
    best.push({"plane", 0.75f});
    CHECK(best.top().label == "drone");

    sub("std::pair");
    std::pair<std::string, int> p{"width", 640};
    auto p2 = std::make_pair(std::string("height"), 480);
    CHECK(p.first == "width" && p2.second == 480);
    CHECK(std::make_pair(1, 9) < std::make_pair(2, 0));   // compares first, then second

    sub("std::tuple (fixed-size, mixed types)    Python: tuple");
    std::tuple<int, std::string, double> t{7, "cam", 29.97};
    CHECK(std::get<0>(t) == 7);               // index must be a compile-time constant
    CHECK(std::get<std::string>(t) == "cam"); // or get by type (if unique)
    int id;
    std::string name;
    double fps;
    std::tie(id, name, fps) = t;              // unpack into existing variables
    auto [id2, name2, fps2] = t;              // unpack into new variables (preferred)
    CHECK(id == id2 && name == name2 && fps == fps2);
    CHECK(std::tuple_size_v<decltype(t)> == 3);
    // Tip: when fields have meaning, a struct with names beats a tuple for readability.

    sub("std::optional");
    std::vector<int> v{4, 8, 15};
    auto found = index_of(v, 8);
    CHECK(found.has_value() && *found == 1);
    auto missing = index_of(v, 99);
    CHECK(!missing);                           // converts to false when empty
    CHECK(missing.value_or(999) == 999);

    sub("std::variant (a type-safe union: one of several types)");
    std::variant<int, std::string> value = 42;
    CHECK(std::holds_alternative<int>(value));
    value = std::string("forty-two");
    CHECK(std::get<std::string>(value) == "forty-two");
    std::string description = std::visit(
        [](const auto& x) -> std::string {
            if constexpr (std::is_same_v<std::decay_t<decltype(x)>, int>) return "int";
            else return "string";
        },
        value);
    CHECK(description == "string");
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Are the brackets balanced? "([]{})" -> true, "(]" -> false, "((" -> false. Use std::stack<char>.
bool is_balanced([[maybe_unused]] const std::string& s) { return false; }  // TODO
// The k smallest values in ascending order, using a priority_queue.
std::vector<int> k_smallest([[maybe_unused]] const std::vector<int>& v, [[maybe_unused]] std::size_t k) {
    return {};  // TODO
}
// Parse "x,y" into a pair; return std::nullopt if the text is malformed (e.g. "12" or "a,b").
std::optional<std::pair<int, int>> parse_point([[maybe_unused]] const std::string& s) {
    return std::nullopt;  // TODO
}

void exercises() {
    CHECK(is_balanced("([]{})"));
    CHECK(!is_balanced("(]"));
    CHECK(!is_balanced("(("));
    CHECK(is_balanced(""));
    CHECK(k_smallest({7, 2, 9, 1, 5}, 3) == std::vector<int>({1, 2, 5}));
    auto pt = parse_point("12,34");
    CHECK(pt.has_value() && pt->first == 12 && pt->second == 34);
    CHECK(!parse_point("12").has_value());
    CHECK(!parse_point("a,b").has_value());
}

}  // namespace s09

// =====================================================================================
// 10  STRUCTS AND CLASSES (OOP 1: encapsulation)
// =====================================================================================
//  Python -> C++ map
//    class Foo:                 -> class Foo { ... };          (note the ; at the end)
//    def __init__(self, ...)    -> constructor  Foo(...)
//    self.x                     -> member x_  (`this` is implicit; this->x_ also works)
//    _private (convention)      -> private:   (enforced by the compiler)
//    @property                  -> getter method, usually const
//    @staticmethod / classmethod-> static member function
//    @dataclass                 -> struct with public fields
//    __repr__ / __str__         -> operator<< (see below)
//  struct vs class: identical, except struct members are public by default and class
//  members are private by default. Convention: struct = plain data, class = has invariants.
namespace s10 {

struct Point {             // aggregate: public fields with default values
    int x = 0;
    int y = 0;
};

struct Size {
    int width = 0;
    int height = 0;
    int area() const { return width * height; }   // const = "does not modify the object"
    bool empty() const { return area() == 0; }
};

class Rect {
public:
    static constexpr int kMaxSide = 100000;        // class-level constant

    Rect() = default;                              // default constructor: 0,0,0,0
    Rect(int x, int y, int w, int h)               // member initializer list (preferred)
        : x_(x), y_(y), w_(w), h_(h) {
        if (w < 0 || h < 0) throw std::invalid_argument("Rect: negative size");
    }
    Rect(Point top_left, Size size)                // delegating constructor
        : Rect(top_left.x, top_left.y, size.width, size.height) {}
    explicit Rect(int side) : Rect(0, 0, side, side) {}   // explicit: no silent int -> Rect

    static Rect from_corners(Point a, Point b) {   // static "named constructor" (factory)
        return Rect(std::min(a.x, b.x), std::min(a.y, b.y), std::abs(b.x - a.x), std::abs(b.y - a.y));
    }

    // getters: const, cheap, return by value
    int x() const { return x_; }
    int y() const { return y_; }
    int width() const { return w_; }
    int height() const { return h_; }
    int area() const { return w_ * h_; }
    Point center() const { return {x_ + w_ / 2, y_ + h_ / 2}; }

    bool contains(Point p) const {
        return p.x >= x_ && p.x < x_ + w_ && p.y >= y_ && p.y < y_ + h_;
    }

    // setter that PROTECTS the invariant (this is what encapsulation is for)
    void set_width(int w) {
        if (w < 0 || w > kMaxSide) throw std::out_of_range("Rect::set_width");
        w_ = w;
    }

    // returning *this by reference allows chaining: r.translate(1, 2).scale(2)
    Rect& translate(int dx, int dy) {
        x_ += dx;
        y_ += dy;
        return *this;
    }
    Rect& scale(int factor) {
        w_ *= factor;
        h_ *= factor;
        return *this;
    }

    // print support: std::cout << rect   (Python __repr__)
    friend std::ostream& operator<<(std::ostream& os, const Rect& r) {
        return os << "Rect(x=" << r.x_ << ", y=" << r.y_ << ", w=" << r.w_ << ", h=" << r.h_ << ")";
    }

private:
    int x_ = 0;            // trailing underscore: common convention for members
    int y_ = 0;
    int w_ = 0;
    int h_ = 0;
};

// A class with a static counter shared by all instances.
class Camera {
public:
    explicit Camera(std::string name) : name_(std::move(name)) { ++instances_; }
    ~Camera() { --instances_; }           // destructor: runs automatically at end of scope
    Camera(const Camera&) = delete;       // a real device must not be copied
    Camera& operator=(const Camera&) = delete;
    const std::string& name() const { return name_; }   // return big members by const&
    static int instances() { return instances_; }
private:
    std::string name_;
    static inline int instances_ = 0;    // C++17 inline static: one shared variable
};

void demo() {
    sub("structs: plain data");
    Point p{3, 4};                        // aggregate initialisation
    Point origin;                         // uses the default member values
    Size s{640, 480};
    CHECK(p.x == 3 && origin.y == 0 && s.area() == 307200);

    sub("class: constructors");
    Rect a;                               // default
    Rect b(10, 20, 30, 40);               // parameterised
    Rect c(Point{1, 1}, Size{2, 3});      // delegating
    Rect sq(5);                           // explicit single-argument
    Rect d = Rect::from_corners({10, 10}, {0, 4});
    CHECK(a.area() == 0 && b.area() == 1200 && c.width() == 2 && sq.height() == 5);
    CHECK(d.x() == 0 && d.y() == 4 && d.width() == 10 && d.height() == 6);
    // Rect bad = 5;                      // compile error thanks to explicit
    std::cout << "    " << b << '\n';

    sub("member functions, const-correctness, invariants");
    CHECK(b.contains({15, 25}) && !b.contains({0, 0}));
    CHECK(b.center().x == 25);
    bool rejected = false;
    try { b.set_width(-1); } catch (const std::out_of_range&) { rejected = true; }
    CHECK(rejected && b.width() == 30);   // the object stayed valid
    b.translate(5, 5).scale(2);           // chaining
    CHECK(b.x() == 15 && b.width() == 60);
    const Rect frozen(0, 0, 2, 2);
    CHECK(frozen.area() == 4);            // OK: area() is const
    // frozen.translate(1, 1);            // compile error: translate() is not const

    sub("static members and destructors (object lifetime)");
    CHECK(Camera::instances() == 0);
    {
        Camera left("left"), right("right");
        CHECK(Camera::instances() == 2);
        CHECK(left.name() == "left");
    }                                     // <- both destructors run here, in reverse order
    CHECK(Camera::instances() == 0);
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Area of the overlap of two rects (0 if they don't overlap). Use only the public getters.
int intersection_area([[maybe_unused]] const Rect& a, [[maybe_unused]] const Rect& b) { return 0; }  // TODO
// Intersection over Union = intersection / (areaA + areaB - intersection). 0 if union is 0.
double iou([[maybe_unused]] const Rect& a, [[maybe_unused]] const Rect& b) { return 0.0; }  // TODO

// Design your own class: a BankAccount whose balance can never go negative.
class BankAccount {
public:
    explicit BankAccount(double initial) : balance_(initial) {}   // TODO: reject initial < 0
    double balance() const { return balance_; }
    void deposit([[maybe_unused]] double amount) {}               // TODO: reject amount <= 0
    bool withdraw([[maybe_unused]] double amount) { return false; }  // TODO: false if not enough money
private:
    double balance_;
};

void exercises() {
    Rect a(0, 0, 10, 10), b(5, 5, 10, 10), far(100, 100, 5, 5);
    CHECK(intersection_area(a, b) == 25);
    CHECK(intersection_area(a, far) == 0);
    CHECK_NEAR(iou(a, b), 25.0 / 175.0, 1e-9);
    CHECK_NEAR(iou(a, a), 1.0, 1e-9);
    BankAccount acc(100);
    acc.deposit(50);
    CHECK(acc.balance() == 150);
    CHECK(acc.withdraw(120) && acc.balance() == 30);
    CHECK(!acc.withdraw(1000) && acc.balance() == 30);
    bool threw = false;
    try { BankAccount bad(-5); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
}

}  // namespace s10

// =====================================================================================
// 11  INHERITANCE AND POLYMORPHISM (OOP 2: inheritance, abstraction, polymorphism)
// =====================================================================================
namespace s11 {

constexpr double kPi = 3.14159265358979323846;

// Abstract base class = interface (Python: class Shape(ABC) with @abstractmethod)
class Shape {
public:
    explicit Shape(std::string name) : name_(std::move(name)) {}
    virtual ~Shape() = default;                   // ALWAYS virtual in a polymorphic base class
    virtual double area() const = 0;              // = 0  ->  pure virtual: must be overridden
    virtual double perimeter() const = 0;
    virtual std::string describe() const {        // virtual with a default implementation
        std::ostringstream os;
        os << name_ << " area=" << area();
        return os.str();
    }
    const std::string& name() const { return name_; }
protected:                                        // visible to derived classes only
    std::string name_;
};

class Rectangle : public Shape {                  // public inheritance: "is-a"
public:
    Rectangle(double w, double h) : Shape("rectangle"), w_(w), h_(h) {}  // call base ctor first
    double area() const override { return w_ * h_; }   // override: compiler checks the signature
    double perimeter() const override { return 2 * (w_ + h_); }
protected:
    double w_, h_;
};

class Square final : public Rectangle {           // final: nobody can derive from Square
public:
    explicit Square(double side) : Rectangle(side, side) { name_ = "square"; }
    std::string describe() const override {
        return "[special] " + Rectangle::describe();  // call the base version explicitly
    }
};

// ---- YOUR TURN (part of the hierarchy, implemented below in exercises) --------------
class Circle : public Shape {
public:
    explicit Circle(double r) : Shape("circle"), r_(r) {}
    double area() const override { return 0.0; }       // TODO: pi r^2
    double perimeter() const override { return 0.0; }  // TODO: 2 pi r
private:
    double r_;
};

// Construction / destruction order, recorded in a log.
std::vector<std::string> g_log;
struct Base {
    Base() { g_log.push_back("Base()"); }
    virtual ~Base() { g_log.push_back("~Base()"); }
};
struct Derived : Base {
    Derived() { g_log.push_back("Derived()"); }
    ~Derived() override { g_log.push_back("~Derived()"); }
};

// Composition ("has-a") is often better than inheritance ("is-a").
class Lens { public: double focal_mm() const { return 25.0; } };
class Sensor { public: int width_px() const { return 640; } };
class ThermalCamera {                              // HAS a lens and a sensor
public:
    double horizontal_fov_deg(double pixel_pitch_um) const {
        double sensor_width_mm = sensor_.width_px() * pixel_pitch_um / 1000.0;
        return 2.0 * std::atan(sensor_width_mm / (2.0 * lens_.focal_mm())) * 180.0 / kPi;
    }
private:
    Lens lens_;
    Sensor sensor_;
};

void demo() {
    sub("polymorphism: one interface, many implementations");
    std::vector<std::unique_ptr<Shape>> shapes;  // own derived objects through base pointers
    shapes.push_back(std::make_unique<Rectangle>(2, 3));
    shapes.push_back(std::make_unique<Square>(4));
    double total = 0;
    for (const auto& s : shapes) {
        std::cout << "    " << s->describe() << '\n';   // calls the right override at runtime
        total += s->area();
    }
    CHECK(total == 6 + 16);
    // Shape s("x");                 // compile error: Shape is abstract

    sub("references to base work the same way");
    Square sq(3);
    const Shape& ref = sq;
    CHECK(ref.area() == 9 && ref.name() == "square");

    sub("dynamic_cast: ask 'is this base pointer really a Square?'");
    int squares = 0;
    for (const auto& s : shapes)
        if (dynamic_cast<const Square*>(s.get()) != nullptr) ++squares;
    CHECK(squares == 1);

    sub("constructor / destructor order");
    g_log.clear();
    {
        std::unique_ptr<Base> obj = std::make_unique<Derived>();
    }   // virtual ~Base() makes ~Derived() run too. Without virtual -> only ~Base(): a bug.
    CHECK(g_log == std::vector<std::string>({"Base()", "Derived()", "~Derived()", "~Base()"}));

    sub("object slicing: copying a derived object INTO a base object loses the derived part");
    Rectangle r = Square(2);          // r is a plain Rectangle now; describe() is not "[special]"
    CHECK(r.describe().find("[special]") == std::string::npos);
    // That's why polymorphic objects are handled through pointers or references.

    sub("composition");
    ThermalCamera cam;
    std::cout << "    HFOV with 12um pixels: " << cam.horizontal_fov_deg(12.0) << " deg\n";
}

void exercises() {
    Circle c(2.0);
    CHECK_NEAR(c.area(), kPi * 4.0, 1e-9);
    CHECK_NEAR(c.perimeter(), 4.0 * kPi, 1e-9);
    const Shape& s = c;
    CHECK(s.describe().rfind("circle", 0) == 0);
}

}  // namespace s11

// =====================================================================================
// 12  OPERATOR OVERLOADING          (Python: __add__, __eq__, __getitem__, __call__ ...)
// =====================================================================================
namespace s12 {

struct Vec2 {
    double x = 0;
    double y = 0;

    Vec2& operator+=(const Vec2& o) { x += o.x; y += o.y; return *this; }   // a += b
    Vec2& operator*=(double s) { x *= s; y *= s; return *this; }
    Vec2 operator-() const { return {-x, -y}; }                             // -a

    // Implement + in terms of +=  (one place to get it right)
    friend Vec2 operator+(Vec2 a, const Vec2& b) { return a += b; }
    friend Vec2 operator-(const Vec2& a, const Vec2& b) { return {a.x - b.x, a.y - b.y}; }
    friend Vec2 operator*(Vec2 a, double s) { return a *= s; }              // a * 2
    friend Vec2 operator*(double s, Vec2 a) { return a *= s; }              // 2 * a
    friend bool operator==(const Vec2& a, const Vec2& b) { return a.x == b.x && a.y == b.y; }
    friend bool operator!=(const Vec2& a, const Vec2& b) { return !(a == b); }

    double& operator[](std::size_t i) {                                     // a[0], a[1]
        if (i > 1) throw std::out_of_range("Vec2 index");
        return i == 0 ? x : y;
    }
    friend std::ostream& operator<<(std::ostream& os, const Vec2& v) {
        return os << "(" << v.x << ", " << v.y << ")";
    }
};

// operator< lets std::sort, std::set and std::map use your type directly (Python __lt__)
struct Detection {
    std::string label;
    float score = 0.f;
    friend bool operator<(const Detection& a, const Detection& b) { return a.score < b.score; }
};

// operator() makes an object callable: a "functor" (Python __call__). It can carry state.
struct AboveThreshold {
    int threshold;
    bool operator()(int value) const { return value > threshold; }
};

void demo() {
    sub("arithmetic and comparison operators");
    Vec2 a{1, 2}, b{3, 4};
    CHECK(a + b == Vec2({4, 6}));
    CHECK(b - a == Vec2({2, 2}));
    CHECK(a * 2.0 == 2.0 * a);
    CHECK(-a == Vec2({-1, -2}));
    a += b;
    CHECK(a == Vec2({4, 6}) && a != b);
    a[1] = 10;
    CHECK(a.y == 10);
    std::cout << "    a = " << a << '\n';

    sub("operator< makes a type sortable");
    std::vector<Detection> dets{{"bird", 0.3f}, {"drone", 0.9f}, {"plane", 0.6f}};
    std::sort(dets.begin(), dets.end());
    CHECK(dets.back().label == "drone");
    CHECK(std::max_element(dets.begin(), dets.end())->label == "drone");

    sub("functors: objects you can call like functions");
    AboveThreshold bright{128};
    CHECK(bright(200) && !bright(50));
    std::vector<int> pixels{10, 140, 250, 90, 129};
    CHECK(std::count_if(pixels.begin(), pixels.end(), bright) == 3);
}

// ---- YOUR TURN ----------------------------------------------------------------------
double dot([[maybe_unused]] const Vec2& a, [[maybe_unused]] const Vec2& b) { return 0.0; }  // TODO
double norm([[maybe_unused]] const Vec2& v) { return 0.0; }  // TODO: length, use dot() + std::sqrt
// Add operator/ (Vec2 / double) yourself below, then uncomment its test in exercises().

void exercises() {
    CHECK(dot({1, 2}, {3, 4}) == 11.0);
    CHECK_NEAR(norm({3, 4}), 5.0, 1e-12);
    // CHECK(Vec2({4, 6}) / 2.0 == Vec2({2, 3}));
}

}  // namespace s12

// =====================================================================================
// 13  MEMORY, RAII, SMART POINTERS, MOVE SEMANTICS
// =====================================================================================
//  Stack: local variables. Fast, freed automatically at the closing }.
//  Heap:  dynamic memory (new, or inside vector/string/unique_ptr). Flexible, slower.
//  RAII ("Resource Acquisition Is Initialization"): an object acquires a resource in its
//  constructor and releases it in its destructor. Since destructors run automatically at
//  the end of scope, cleanup can never be forgotten — even when an exception is thrown.
//  This replaces Python's garbage collector AND its `with` statement.
namespace s13 {

// A buffer that manages raw memory by hand. Writing all five special members is the
// "Rule of Five". You rarely do this in practice — but you must understand it.
struct BufferStats { int constructed = 0, copies = 0, moves = 0; };

class Buffer {
public:
    static inline BufferStats stats{};     // shared counters so the demo can SHOW copies vs moves

    explicit Buffer(std::size_t n) : size_(n), data_(n ? new int[n]() : nullptr) { ++stats.constructed; }
    ~Buffer() { delete[] data_; }                                          // 1. destructor

    Buffer(const Buffer& other)                                           // 2. copy constructor
        : size_(other.size_), data_(other.size_ ? new int[other.size_] : nullptr) {
        if (size_) std::copy(other.data_, other.data_ + size_, data_);    // deep copy
        ++stats.copies;
    }
    Buffer& operator=(const Buffer& other) {                              // 3. copy assignment
        if (this != &other) {
            Buffer tmp(other);    // copy-and-swap: if the copy throws, *this is untouched
            swap(tmp);
        }
        return *this;
    }
    Buffer(Buffer&& other) noexcept                                       // 4. move constructor
        : size_(other.size_), data_(other.data_) {                        //    steal the pointer
        other.size_ = 0;
        other.data_ = nullptr;                                            //    leave source empty
        ++stats.moves;
    }
    Buffer& operator=(Buffer&& other) noexcept {                          // 5. move assignment
        if (this != &other) {
            delete[] data_;
            data_ = std::exchange(other.data_, nullptr);
            size_ = std::exchange(other.size_, 0);
            ++stats.moves;
        }
        return *this;
    }
    void swap(Buffer& other) noexcept {
        std::swap(size_, other.size_);
        std::swap(data_, other.data_);
    }
    std::size_t size() const { return size_; }
    int& operator[](std::size_t i) { return data_[i]; }
    const int& operator[](std::size_t i) const { return data_[i]; }

private:
    std::size_t size_;     // declared before data_: members initialise in declaration order
    int* data_;
};

// Rule of Zero: let members that already manage themselves do the work.
// No destructor, no copy/move code — and it is correct automatically. PREFER THIS.
class SafeBuffer {
public:
    explicit SafeBuffer(std::size_t n) : data_(n) {}
    std::size_t size() const { return data_.size(); }
private:
    std::vector<int> data_;
};

Buffer make_buffer(std::size_t n) { return Buffer(n); }   // returned without copying (RVO)

void demo() {
    sub("copy vs move (counted by Buffer::stats)");
    Buffer::stats = BufferStats{};
    Buffer a(1000);
    a[0] = 7;
    Buffer b = a;                         // copy: allocates and copies 1000 ints
    Buffer c = std::move(a);              // move: steals a's pointer, no allocation
    CHECK(Buffer::stats.copies == 1 && Buffer::stats.moves == 1);
    CHECK(b[0] == 7 && c[0] == 7);
    CHECK(a.size() == 0);                 // moved-from: valid but empty. Don't use its data.
    Buffer d = make_buffer(10);           // return value optimisation: no copy, no move
    CHECK(Buffer::stats.copies == 1 && d.size() == 10);
    std::vector<Buffer> pool;
    pool.reserve(2);
    pool.push_back(Buffer(5));            // temporary -> moved into the vector
    pool.emplace_back(5);                 // constructed directly inside the vector
    CHECK(Buffer::stats.copies == 1);
    // std::move does not move anything by itself: it is a cast that says
    // "you may steal from this object". The move constructor does the stealing.

    sub("Rule of Zero");
    SafeBuffer s1(100);
    SafeBuffer s2 = s1;                   // vector copies itself correctly
    CHECK(s2.size() == 100);

    sub("std::unique_ptr: single owner, zero overhead. The default smart pointer.");
    std::unique_ptr<int> up = std::make_unique<int>(42);
    CHECK(*up == 42);
    // std::unique_ptr<int> up2 = up;                // compile error: cannot copy an owner
    std::unique_ptr<int> up2 = std::move(up);        // ownership transferred
    CHECK(up == nullptr && *up2 == 42);
    auto arr = std::make_unique<float[]>(4);         // dynamic array, freed automatically
    arr[3] = 1.5f;
    CHECK(arr[3] == 1.5f);
    up2.reset();                                     // free now (also happens at scope end)
    CHECK(!up2);

    sub("std::shared_ptr: shared ownership with a reference count (has a cost)");
    std::shared_ptr<std::string> model = std::make_shared<std::string>("yolo.onnx");
    {
        std::shared_ptr<std::string> user = model;   // copy: count goes up
        CHECK(model.use_count() == 2);
    }                                                // user destroyed: count back down
    CHECK(model.use_count() == 1);
    std::weak_ptr<std::string> observer = model;     // watches without owning (breaks cycles)
    if (auto locked = observer.lock()) CHECK(*locked == "yolo.onnx");
    model.reset();                                   // last owner gone -> string freed
    CHECK(observer.expired());

    sub("RAII in action: cleanup happens even when an exception is thrown");
    Buffer::stats = BufferStats{};
    try {
        Buffer temp(10);
        throw std::runtime_error("something failed");
    } catch (const std::runtime_error&) {
        // temp's destructor already ran here: no leak
    }
    CHECK(Buffer::stats.constructed == 1);

    // Classic bugs RAII and smart pointers prevent:
    //   leak          new without delete
    //   double free   delete twice
    //   dangling      using memory after it was freed, e.g. returning a reference to a local:
    //                 const std::string& bad() { std::string s = "x"; return s; }   // NO
}

// ---- YOUR TURN: predict, then verify ------------------------------------------------
// Replace each -1 with your prediction BEFORE running. Then run "13 ex" to check yourself.
constexpr int kPredictCopies = -1;   // TODO
constexpr int kPredictMoves = -1;    // TODO

void take_by_value(Buffer) {}
void take_by_cref(const Buffer&) {}

void exercises() {
    Buffer::stats = BufferStats{};
    Buffer x(8);
    take_by_value(x);              // ?
    take_by_cref(x);               // ?
    take_by_value(std::move(x));   // ?
    Buffer y(3);
    y = Buffer(4);                 // ?
    CHECK(Buffer::stats.copies == kPredictCopies);
    CHECK(Buffer::stats.moves == kPredictMoves);
}

}  // namespace s13

// =====================================================================================
// 14  TEMPLATES (generic code — like Python's duck typing, but checked at compile time)
// =====================================================================================
namespace s14 {

// Function template: works for any T that supports <
template <typename T>
T max_of(const T& a, const T& b) { return (a < b) ? b : a; }

// Sum any vector: T{} is "zero" for numbers and "" for strings
template <typename T>
T sum_all(const std::vector<T>& v) { return std::accumulate(v.begin(), v.end(), T{}); }

// Class template: a stack of anything
template <typename T>
class Stack {
public:
    void push(const T& value) { items_.push_back(value); }
    void push(T&& value) { items_.push_back(std::move(value)); }
    T pop() {
        if (items_.empty()) throw std::out_of_range("pop from empty Stack");
        T value = std::move(items_.back());
        items_.pop_back();
        return value;
    }
    bool empty() const { return items_.empty(); }
    std::size_t size() const { return items_.size(); }
private:
    std::vector<T> items_;
};

// Non-type template parameter: a value known at compile time (like std::array<T, N>)
template <typename T, std::size_t N>
constexpr std::size_t length_of(const T (&)[N]) { return N; }

// constexpr function: can run at compile time
constexpr int cube(int x) { return x * x * x; }
static_assert(cube(3) == 27, "evaluated by the compiler");

// if constexpr: choose code per type at compile time
template <typename T>
std::string kind_of() {
    if constexpr (std::is_integral_v<T>) return "integer";
    else if constexpr (std::is_floating_point_v<T>) return "floating";
    else return "other";
}

// Two type parameters
template <typename K, typename V>
struct KeyValue {
    K key;
    V value;
};

void demo() {
    sub("function templates: the compiler generates one version per type used");
    CHECK(max_of(3, 7) == 7);
    CHECK(max_of(2.5, 1.5) == 2.5);
    CHECK(max_of(std::string("abc"), std::string("abd")) == "abd");
    CHECK(max_of<double>(3, 2.5) == 3.0);       // explicit type when deduction is ambiguous
    CHECK(sum_all(std::vector<int>{1, 2, 3}) == 6);
    CHECK(sum_all(std::vector<std::string>{"a", "b"}) == "ab");

    sub("class templates");
    Stack<std::string> names;
    names.push("first");
    names.push("second");
    CHECK(names.pop() == "second" && names.size() == 1);
    Stack<int> numbers;
    bool threw = false;
    try { numbers.pop(); } catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);

    sub("compile-time values and if constexpr");
    int raw[7] = {};
    static_assert(length_of(raw) == 7, "known at compile time");
    CHECK(kind_of<int>() == "integer" && kind_of<float>() == "floating" && kind_of<std::string>() == "other");
    KeyValue<std::string, int> kv{"fps", 30};
    CHECK(kv.key == "fps" && kv.value == 30);
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Generic clamp: works for int, double, uint8_t...
template <typename T>
T clamp_value([[maybe_unused]] T v, [[maybe_unused]] T lo, [[maybe_unused]] T hi) { return v; }  // TODO
// How many elements are strictly greater than threshold?
template <typename T>
std::size_t count_greater([[maybe_unused]] const std::vector<T>& v, [[maybe_unused]] const T& threshold) {
    return 0;  // TODO
}
// A generic Pair-like Range<T> with lo/hi and contains(x). Fill in contains().
template <typename T>
struct Range {
    T lo, hi;
    bool contains([[maybe_unused]] const T& x) const { return false; }  // TODO: lo <= x <= hi
};

void exercises() {
    CHECK(clamp_value(300, 0, 255) == 255);
    CHECK(clamp_value(-0.5, 0.0, 1.0) == 0.0);
    CHECK(count_greater(std::vector<int>{1, 5, 9, 12}, 5) == 2);
    CHECK(count_greater(std::vector<double>{0.1, 0.9}, 0.5) == 1);
    Range<int> r{10, 20};
    CHECK(r.contains(10) && r.contains(20) && !r.contains(21));
}

}  // namespace s14

// =====================================================================================
// 15  STL ALGORITHMS  (#include <algorithm> and <numeric>) — learn these instead of loops
// =====================================================================================
namespace s15 {

struct Person {
    std::string name;
    int age;
};

void demo() {
    std::vector<int> v{5, 3, 8, 1, 9, 2, 7};

    sub("min / max / sum / count");
    CHECK(*std::min_element(v.begin(), v.end()) == 1);          // min(v)
    CHECK(*std::max_element(v.begin(), v.end()) == 9);          // max(v)
    auto [mn, mx] = std::minmax_element(v.begin(), v.end());
    CHECK(*mn == 1 && *mx == 9);
    CHECK(std::accumulate(v.begin(), v.end(), 0) == 35);        // sum(v)
    CHECK(std::count_if(v.begin(), v.end(), [](int x) { return x > 4; }) == 4);

    sub("any / all / none        Python: any(), all()");
    CHECK(std::any_of(v.begin(), v.end(), [](int x) { return x > 8; }));
    CHECK(std::all_of(v.begin(), v.end(), [](int x) { return x > 0; }));
    CHECK(std::none_of(v.begin(), v.end(), [](int x) { return x < 0; }));

    sub("find_if: first element matching a condition");
    auto it = std::find_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    CHECK(it != v.end() && *it == 8);

    sub("transform = map()   and   copy_if = filter()");
    std::vector<int> squared(v.size());
    std::transform(v.begin(), v.end(), squared.begin(), [](int x) { return x * x; });
    CHECK(squared[0] == 25);
    std::vector<int> evens;
    std::copy_if(v.begin(), v.end(), std::back_inserter(evens), [](int x) { return x % 2 == 0; });
    CHECK(evens == std::vector<int>({8, 2}));

    sub("sorting with a key       Python: sorted(people, key=lambda p: p.age)");
    std::vector<Person> people{{"ana", 31}, {"ben", 25}, {"cem", 31}, {"dan", 19}};
    std::stable_sort(people.begin(), people.end(),     // stable: equal ages keep their order
                     [](const Person& a, const Person& b) { return a.age < b.age; });
    CHECK(people[0].name == "dan" && people[2].name == "ana" && people[3].name == "cem");

    sub("partial_sort (top-k) and nth_element (median) — faster than a full sort");
    std::vector<int> w = v;
    std::partial_sort(w.begin(), w.begin() + 3, w.end(), std::greater<int>());
    CHECK(w[0] == 9 && w[1] == 8 && w[2] == 7);
    std::vector<int> m = v;
    std::nth_element(m.begin(), m.begin() + m.size() / 2, m.end());
    CHECK(m[m.size() / 2] == 5);                                 // median of 7 values

    sub("unique (remove ADJACENT duplicates: sort first), reverse, iota");
    std::vector<int> d{3, 1, 3, 2, 1, 2};
    std::sort(d.begin(), d.end());
    d.erase(std::unique(d.begin(), d.end()), d.end());          // sorted(set(d))
    CHECK(d == std::vector<int>({1, 2, 3}));
    std::reverse(d.begin(), d.end());
    CHECK(d.front() == 3);
    std::vector<int> r(5);
    std::iota(r.begin(), r.end(), 10);                           // 10, 11, 12, 13, 14
    CHECK(r.back() == 14);

    sub("binary search on SORTED data: O(log n)");
    std::vector<int> sorted{1, 3, 5, 7, 9, 11};
    CHECK(std::binary_search(sorted.begin(), sorted.end(), 7));
    auto lb = std::lower_bound(sorted.begin(), sorted.end(), 6);  // first >= 6  (bisect_left)
    CHECK(*lb == 7 && (lb - sorted.begin()) == 3);

    sub("inner_product (dot product) and partial_sum (prefix sums / cumsum)");
    std::vector<int> a{1, 2, 3}, b{4, 5, 6};
    CHECK(std::inner_product(a.begin(), a.end(), b.begin(), 0) == 32);
    std::vector<int> prefix(a.size());
    std::partial_sum(a.begin(), a.end(), prefix.begin());
    CHECK(prefix == std::vector<int>({1, 3, 6}));
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Use algorithms, not hand-written loops.
// Largest k values, in descending order. (partial_sort)
std::vector<int> top_k([[maybe_unused]] std::vector<int> v, [[maybe_unused]] std::size_t k) { return {}; }  // TODO
// Mean of all values above threshold, 0.0 if none. (copy_if + accumulate, or a single accumulate)
double mean_above([[maybe_unused]] const std::vector<double>& v, [[maybe_unused]] double threshold) {
    return 0.0;  // TODO
}
// Names of people older than min_age, sorted alphabetically.
std::vector<std::string> names_older_than([[maybe_unused]] const std::vector<Person>& people,
                                          [[maybe_unused]] int min_age) {
    return {};  // TODO
}

void exercises() {
    CHECK(top_k({5, 3, 8, 1, 9}, 2) == std::vector<int>({9, 8}));
    CHECK_NEAR(mean_above({0.2, 0.8, 0.6, 0.1}, 0.5), 0.7, 1e-12);
    CHECK(mean_above({0.1}, 0.5) == 0.0);
    std::vector<Person> p{{"zoe", 40}, {"amy", 33}, {"tom", 12}};
    CHECK(names_older_than(p, 18) == std::vector<std::string>({"amy", "zoe"}));
}

}  // namespace s15

// =====================================================================================
// 16  ERRORS, MATH AND FILES
// =====================================================================================
namespace s16 {

// A custom exception type: derive from a standard one so `catch (const std::exception&)`
// still catches it.
class SensorError : public std::runtime_error {
public:
    SensorError(const std::string& message, int code) : std::runtime_error(message), code_(code) {}
    int code() const noexcept { return code_; }
private:
    int code_;
};

double checked_sqrt(double x) {
    if (x < 0) throw std::domain_error("checked_sqrt: negative input");   // raise ValueError
    return std::sqrt(x);
}

void read_sensor(int id) {
    if (id < 0) throw SensorError("sensor offline", 503);
}

void demo() {
    sub("try / catch      Python: try / except. Catch by const reference, specific first.");
    std::string caught;
    try {
        read_sensor(-1);
    } catch (const SensorError& e) {          // most specific first
        caught = std::string(e.what()) + " code=" + std::to_string(e.code());
    } catch (const std::exception& e) {       // then the general base class
        caught = e.what();
    } catch (...) {                           // anything at all (rarely useful)
        caught = "unknown";
    }
    CHECK(caught == "sensor offline code=503");
    bool domain = false;
    try { checked_sqrt(-1); } catch (const std::domain_error&) { domain = true; }
    CHECK(domain);
    // noexcept on a function = promise it never throws (moves, destructors, swap).
    // In real-time / embedded code exceptions are often disabled: return error codes,
    // bool + output, or std::optional instead (see section 9).

    sub("<cmath>");
    CHECK(std::sqrt(16.0) == 4.0 && std::pow(2.0, 10) == 1024.0);
    CHECK(std::abs(-3) == 3 && std::fabs(-2.5) == 2.5);
    CHECK(std::floor(2.7) == 2.0 && std::ceil(2.1) == 3.0 && std::round(2.5) == 3.0);
    CHECK(std::trunc(-2.7) == -2.0);
    CHECK(std::hypot(3.0, 4.0) == 5.0);                     // sqrt(x^2 + y^2) without overflow
    const double pi = std::acos(-1.0);                      // M_PI is not standard C++
    CHECK_NEAR(std::atan2(1.0, 1.0), pi / 4, 1e-12);        // angle of (x=1, y=1)
    CHECK_NEAR(std::sin(pi / 2), 1.0, 1e-12);               // trig uses RADIANS
    CHECK_NEAR(std::exp(std::log(10.0)), 10.0, 1e-12);
    CHECK(std::isnan(std::nan("")) && std::isfinite(1.0));
    CHECK(std::clamp(300, 0, 255) == 255 && std::min({4, 2, 8}) == 2);
    CHECK_NEAR(0.1 + 0.2, 0.3, 1e-12);                      // never compare floats with ==
    CHECK(0.1 + 0.2 != 0.3);

    sub("files: write a CSV, read it back, append, binary I/O");
    const std::string path = "cheatsheet_demo.csv";
    {
        std::ofstream out(path);                            // open for writing (truncates)
        if (!out) throw std::runtime_error("cannot open " + path);
        out << "frame,x,y\n";
        out << "0,10,20\n";
        out << "1,12,21\n";
    }                                                       // closed automatically (RAII)
    {
        std::ofstream out(path, std::ios::app);             // append mode
        out << "2,15,23\n";
    }
    std::vector<std::vector<std::string>> rows;
    {
        std::ifstream in(path);
        std::string line;
        std::getline(in, line);                             // skip the header
        while (std::getline(in, line)) {                    // for line in file:
            std::vector<std::string> fields;
            std::string field;
            std::istringstream ls(line);
            while (std::getline(ls, field, ',')) fields.push_back(field);
            rows.push_back(fields);
        }
    }
    CHECK(rows.size() == 3 && rows[2][1] == "15");

    const std::string raw_path = "cheatsheet_demo.raw";     // raw image bytes
    std::vector<std::uint8_t> image{0, 64, 128, 255};
    {
        std::ofstream out(raw_path, std::ios::binary);
        out.write(reinterpret_cast<const char*>(image.data()), static_cast<std::streamsize>(image.size()));
    }
    std::vector<std::uint8_t> loaded(4);
    {
        std::ifstream in(raw_path, std::ios::binary);
        in.read(reinterpret_cast<char*>(loaded.data()), static_cast<std::streamsize>(loaded.size()));
    }
    CHECK(loaded == image);
    std::remove(path.c_str());                              // delete the demo files
    std::remove(raw_path.c_str());
}

// ---- YOUR TURN ----------------------------------------------------------------------
// "1, 2,3" -> {1,2,3}. Throw std::invalid_argument if any token is not an integer.
// (std::stoi already throws std::invalid_argument for "abc" — but what about "12abc"?
//  Use stoi's second argument `std::size_t* pos` to detect leftover characters.)
std::vector<int> parse_ints([[maybe_unused]] const std::string& csv) { return {}; }  // TODO
// Same, but never throws: returns fallback for bad input. Use try/catch around parse_ints.
std::vector<int> parse_ints_or([[maybe_unused]] const std::string& csv,
                               [[maybe_unused]] const std::vector<int>& fallback) { return {}; }  // TODO

void exercises() {
    CHECK(parse_ints("1, 2,3") == std::vector<int>({1, 2, 3}));
    bool threw = false;
    try { parse_ints("1,abc"); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
    threw = false;
    try { parse_ints("12abc"); } catch (const std::invalid_argument&) { threw = true; }
    CHECK(threw);
    CHECK(parse_ints_or("x", {0}) == std::vector<int>({0}));
}

}  // namespace s16

// =====================================================================================
// 17  MATRIX CLASS AND IMAGE OPERATIONS (CV)
// =====================================================================================
//  A grayscale image IS a matrix: rows = height (y), cols = width (x).
//  Storage is ONE contiguous vector in row-major order:  index = row * cols + col.
//  That is exactly how OpenCV's cv::Mat, NumPy arrays and camera buffers are laid out.
namespace s17 {

template <typename T>
class Matrix {
public:
    Matrix() = default;
    Matrix(std::size_t rows, std::size_t cols, T value = T{})   // Matrix<int> m(2, 3) -> 2x3 zeros
        : rows_(rows), cols_(cols), data_(rows * cols, value) {}
    Matrix(std::initializer_list<std::initializer_list<T>> init)   // Matrix<int> m{{1, 2}, {3, 4}}
        : rows_(init.size()), cols_(init.size() ? init.begin()->size() : 0) {
        data_.reserve(rows_ * cols_);
        for (const auto& row : init) {
            if (row.size() != cols_) throw std::invalid_argument("Matrix: rows of different length");
            data_.insert(data_.end(), row.begin(), row.end());
        }
    }
    // NOTE: Matrix<int> m(2, 3) = a 2x3 matrix. Matrix<int> m{{2, 3}} = a 1x2 matrix holding 2, 3.
    // Parentheses = sizes, braces = values. The same trap exists with std::vector.

    static Matrix identity(std::size_t n) {
        Matrix m(n, n);
        for (std::size_t i = 0; i < n; ++i) m(i, i) = T{1};
        return m;
    }

    std::size_t rows() const { return rows_; }
    std::size_t cols() const { return cols_; }
    std::size_t size() const { return data_.size(); }
    bool same_shape(const Matrix& o) const { return rows_ == o.rows_ && cols_ == o.cols_; }

    // Fast unchecked access (hot loops) and checked access (everything else)
    T& operator()(std::size_t r, std::size_t c) { return data_[r * cols_ + c]; }
    const T& operator()(std::size_t r, std::size_t c) const { return data_[r * cols_ + c]; }
    T& at(std::size_t r, std::size_t c) {
        if (r >= rows_ || c >= cols_) throw std::out_of_range("Matrix::at");
        return (*this)(r, c);
    }
    const T& at(std::size_t r, std::size_t c) const {
        if (r >= rows_ || c >= cols_) throw std::out_of_range("Matrix::at");
        return (*this)(r, c);
    }
    T* data() { return data_.data(); }                     // raw pointer, e.g. to hand to a C API
    const T* data() const { return data_.data(); }

    Matrix transpose() const {
        Matrix out(cols_, rows_);
        for (std::size_t r = 0; r < rows_; ++r)
            for (std::size_t c = 0; c < cols_; ++c) out(c, r) = (*this)(r, c);
        return out;
    }

    Matrix& operator+=(const Matrix& o) {
        require_same_shape(o, "+");
        for (std::size_t i = 0; i < data_.size(); ++i) data_[i] += o.data_[i];
        return *this;
    }
    Matrix& operator-=(const Matrix& o) {
        require_same_shape(o, "-");
        for (std::size_t i = 0; i < data_.size(); ++i) data_[i] -= o.data_[i];
        return *this;
    }
    Matrix& operator*=(T s) {
        for (T& v : data_) v *= s;
        return *this;
    }
    friend Matrix operator+(Matrix a, const Matrix& b) { return a += b; }
    friend Matrix operator-(Matrix a, const Matrix& b) { return a -= b; }
    friend Matrix operator*(Matrix a, T s) { return a *= s; }

    // Matrix product. Loop order i-k-j: the innermost loop walks b and out row by row,
    // i.e. through contiguous memory. The textbook i-j-k order jumps a whole row in b at
    // every step and is several times slower on big matrices (cache misses).
    friend Matrix operator*(const Matrix& a, const Matrix& b) {
        if (a.cols_ != b.rows_) throw std::invalid_argument("Matrix *: inner dimensions differ");
        Matrix out(a.rows_, b.cols_);
        for (std::size_t i = 0; i < a.rows_; ++i)
            for (std::size_t k = 0; k < a.cols_; ++k) {
                const T aik = a(i, k);
                for (std::size_t j = 0; j < b.cols_; ++j) out(i, j) += aik * b(k, j);
            }
        return out;
    }

    Matrix hadamard(const Matrix& o) const {               // element-wise product (NumPy a * b)
        require_same_shape(o, "hadamard");
        Matrix out(*this);
        for (std::size_t i = 0; i < data_.size(); ++i) out.data_[i] *= o.data_[i];
        return out;
    }

    template <typename F>
    Matrix apply(F f) const {                              // apply f to every element
        Matrix out(*this);
        for (T& v : out.data_) v = f(v);
        return out;
    }

    friend bool operator==(const Matrix& a, const Matrix& b) {
        return a.rows_ == b.rows_ && a.cols_ == b.cols_ && a.data_ == b.data_;
    }
    friend bool operator!=(const Matrix& a, const Matrix& b) { return !(a == b); }

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m) {
        for (std::size_t r = 0; r < m.rows_; ++r) {
            os << "    [";
            for (std::size_t c = 0; c < m.cols_; ++c) {
                if (c) os << ", ";
                if constexpr (sizeof(T) == 1) os << std::setw(3) << +m(r, c);  // uint8 would print as a char
                else os << m(r, c);
            }
            os << "]\n";
        }
        return os;
    }

private:
    void require_same_shape(const Matrix& o, const char* op) const {
        if (!same_shape(o)) throw std::invalid_argument(std::string("Matrix ") + op + ": shape mismatch");
    }
    std::size_t rows_ = 0;
    std::size_t cols_ = 0;
    std::vector<T> data_;
};

using Image = Matrix<std::uint8_t>;     // 8-bit grayscale image
using FloatImage = Matrix<float>;       // intermediate results that can be negative or > 255
using Kernel = Matrix<float>;           // filter weights
using Mat3 = Matrix<double>;            // 3x3 geometric transform

std::uint8_t saturate_u8(float v) {     // round and clamp to 0..255 (OpenCV saturate_cast)
    return static_cast<std::uint8_t>(std::clamp(std::lround(v), 0L, 255L));
}

// 2D filtering (cross-correlation, like cv::filter2D). Border: replicate the edge pixel.
FloatImage filter2d(const Image& img, const Kernel& k) {
    if (k.rows() % 2 == 0 || k.cols() % 2 == 0) throw std::invalid_argument("kernel size must be odd");
    const int h = static_cast<int>(img.rows()), w = static_cast<int>(img.cols());
    const int ry = static_cast<int>(k.rows() / 2), rx = static_cast<int>(k.cols() / 2);
    FloatImage out(img.rows(), img.cols());
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            float acc = 0.0f;
            for (int dy = -ry; dy <= ry; ++dy) {
                for (int dx = -rx; dx <= rx; ++dx) {
                    const int sy = std::clamp(y + dy, 0, h - 1);   // replicate border
                    const int sx = std::clamp(x + dx, 0, w - 1);
                    acc += k(dy + ry, dx + rx) * img(sy, sx);
                }
            }
            out(y, x) = acc;
        }
    }
    return out;
}

Image to_u8(const FloatImage& f) {
    Image out(f.rows(), f.cols());
    for (std::size_t r = 0; r < f.rows(); ++r)
        for (std::size_t c = 0; c < f.cols(); ++c) out(r, c) = saturate_u8(f(r, c));
    return out;
}

Image convolve(const Image& img, const Kernel& k) { return to_u8(filter2d(img, k)); }

Kernel box_kernel(std::size_t n) { return Kernel(n, n, 1.0f / static_cast<float>(n * n)); }
Kernel gaussian3() { return Kernel{{1, 2, 1}, {2, 4, 2}, {1, 2, 1}} * (1.0f / 16.0f); }
Kernel sobel_x() { return {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}}; }
Kernel sobel_y() { return sobel_x().transpose(); }

FloatImage gradient_magnitude(const Image& img) {
    const FloatImage gx = filter2d(img, sobel_x());
    const FloatImage gy = filter2d(img, sobel_y());
    FloatImage mag(img.rows(), img.cols());
    for (std::size_t r = 0; r < img.rows(); ++r)
        for (std::size_t c = 0; c < img.cols(); ++c) mag(r, c) = std::hypot(gx(r, c), gy(r, c));
    return mag;
}

// Colour images are usually INTERLEAVED (HWC): R,G,B of one pixel sit next to each other.
// (OpenCV uses BGR order. DNNs usually want planar CHW float — a classic conversion step.)
struct RgbImage {
    std::size_t width = 0, height = 0;
    std::vector<std::uint8_t> data;                     // size = width * height * 3
    RgbImage(std::size_t w, std::size_t h) : width(w), height(h), data(w * h * 3, 0) {}
    std::uint8_t& at(std::size_t x, std::size_t y, std::size_t ch) { return data[(y * width + x) * 3 + ch]; }
    std::uint8_t at(std::size_t x, std::size_t y, std::size_t ch) const { return data[(y * width + x) * 3 + ch]; }
};

Image to_gray(const RgbImage& rgb) {
    Image gray(rgb.height, rgb.width);
    for (std::size_t y = 0; y < rgb.height; ++y)
        for (std::size_t x = 0; x < rgb.width; ++x)
            gray(y, x) = saturate_u8(0.299f * rgb.at(x, y, 0) + 0.587f * rgb.at(x, y, 1) + 0.114f * rgb.at(x, y, 2));
    return gray;
}

// 2D geometry with 3x3 homogeneous matrices: a point (x, y) becomes the column (x, y, 1).
Mat3 translation(double tx, double ty) { return {{1, 0, tx}, {0, 1, ty}, {0, 0, 1}}; }
Mat3 scaling(double sx, double sy) { return {{sx, 0, 0}, {0, sy, 0}, {0, 0, 1}}; }
Mat3 rotation(double radians) {
    const double c = std::cos(radians), s = std::sin(radians);
    return {{c, -s, 0}, {s, c, 0}, {0, 0, 1}};
}
s12::Vec2 transform_point(const Mat3& m, const s12::Vec2& p) {
    const Matrix<double> column{{p.x}, {p.y}, {1.0}};
    const Matrix<double> r = m * column;
    return {r(0, 0), r(1, 0)};
}

// ---------- OOP + CV: a tiny perception-style pipeline of polymorphic filters ---------
class ImageFilter {
public:
    virtual ~ImageFilter() = default;
    virtual Image apply(const Image& in) const = 0;
    virtual std::string name() const = 0;
};

class BlurFilter final : public ImageFilter {
public:
    explicit BlurFilter(std::size_t size) : kernel_(box_kernel(size)), size_(size) {}
    Image apply(const Image& in) const override { return convolve(in, kernel_); }
    std::string name() const override { return "blur" + std::to_string(size_) + "x" + std::to_string(size_); }
private:
    Kernel kernel_;
    std::size_t size_;
};

class InvertFilter final : public ImageFilter {
public:
    Image apply(const Image& in) const override {
        return in.apply([](std::uint8_t v) { return static_cast<std::uint8_t>(255 - v); });
    }
    std::string name() const override { return "invert"; }
};

class Pipeline {
public:
    Pipeline& add(std::unique_ptr<ImageFilter> stage) {
        stages_.push_back(std::move(stage));
        return *this;
    }
    Image run(const Image& input) const {
        Image img = input;
        for (const auto& stage : stages_) img = stage->apply(img);
        return img;
    }
    std::string describe() const {
        std::string out;
        for (const auto& stage : stages_) out += (out.empty() ? "" : " -> ") + stage->name();
        return out;
    }
private:
    std::vector<std::unique_ptr<ImageFilter>> stages_;
};

void demo() {
    sub("matrix basics: row-major storage");
    Matrix<int> a{{1, 2, 3}, {4, 5, 6}};             // 2 rows x 3 cols
    CHECK(a.rows() == 2 && a.cols() == 3 && a(1, 2) == 6);
    CHECK(a.data()[1 * 3 + 2] == 6);                  // the same element, by raw index
    Matrix<int> zeros(2, 3);
    CHECK(zeros.size() == 6 && zeros(1, 1) == 0);
    bool threw = false;
    try { (void)a.at(5, 0); } catch (const std::out_of_range&) { threw = true; }
    CHECK(threw);

    sub("transpose, +, scalar *, element-wise *");
    Matrix<int> at = a.transpose();
    CHECK(at.rows() == 3 && at(2, 1) == 6);
    CHECK(a + a == a * 2);
    CHECK(a.hadamard(a)(1, 1) == 25);

    sub("matrix multiplication: (2x3)(3x2) = 2x2");
    Matrix<int> b{{7, 8}, {9, 10}, {11, 12}};
    Matrix<int> c = a * b;
    CHECK(c == Matrix<int>({{58, 64}, {139, 154}}));
    CHECK(a * Matrix<int>::identity(3) == a);
    std::cout << c;
    threw = false;
    try { (void)(a * a); } catch (const std::invalid_argument&) { threw = true; }   // (2x3)(2x3)
    CHECK(threw);

    sub("2D transforms: rotate, scale, translate (applied right to left)");
    const double pi = std::acos(-1.0);
    s12::Vec2 p = transform_point(rotation(pi / 2), {1, 0});
    CHECK_NEAR(p.x, 0.0, 1e-12);
    CHECK_NEAR(p.y, 1.0, 1e-12);
    Mat3 combo = translation(10, 5) * rotation(pi / 2) * scaling(2, 2);
    s12::Vec2 q = transform_point(combo, {1, 0});   // (1,0) -> scale (2,0) -> rotate (0,2) -> move (10,7)
    CHECK_NEAR(q.x, 10.0, 1e-9);
    CHECK_NEAR(q.y, 7.0, 1e-9);

    sub("image filtering: blur a single bright pixel");
    Image img(5, 5, 0);
    img(2, 2) = 255;
    Image box = convolve(img, box_kernel(3));
    CHECK(box(2, 2) == 28 && box(1, 1) == 28 && box(0, 0) == 0);   // 255 / 9 = 28.3
    Image g = convolve(img, gaussian3());
    CHECK(g(2, 2) == 64 && g(2, 1) == 32 && g(1, 1) == 16);         // weights 4/16, 2/16, 1/16
    std::cout << g;

    sub("edges: Sobel gradient magnitude on a vertical step");
    Image step(4, 6, 0);
    for (std::size_t r = 0; r < step.rows(); ++r)
        for (std::size_t col = 3; col < step.cols(); ++col) step(r, col) = 200;
    FloatImage mag = gradient_magnitude(step);
    CHECK(mag(1, 0) == 0.0f);                 // flat region: no edge
    CHECK(mag(1, 2) == 800.0f);               // 200 * (1 + 2 + 1): the step is right here
    CHECK(mag(1, 5) == 0.0f);

    sub("interleaved RGB (HWC) -> grayscale");
    RgbImage rgb(2, 1);
    rgb.at(0, 0, 0) = 255;                    // pixel 0: pure red
    for (std::size_t ch = 0; ch < 3; ++ch) rgb.at(1, 0, ch) = 255;   // pixel 1: white
    Image gray = to_gray(rgb);
    CHECK(gray(0, 0) == 76 && gray(0, 1) == 255);

    sub("OOP: a pipeline of polymorphic filter stages");
    Pipeline pipeline;
    pipeline.add(std::make_unique<BlurFilter>(3)).add(std::make_unique<InvertFilter>());
    Image out = pipeline.run(img);
    CHECK(out(0, 0) == 255 && out(2, 2) == 255 - 28);
    std::cout << "    pipeline: " << pipeline.describe() << '\n';
}

// ---- YOUR TURN ----------------------------------------------------------------------
// Binary threshold: 255 where pixel > t, else 0. (Hint: Image::apply with a lambda capturing t)
Image threshold([[maybe_unused]] const Image& img, [[maybe_unused]] std::uint8_t t) { return img; }  // TODO
// Count of each intensity 0..255.
std::array<int, 256> histogram([[maybe_unused]] const Image& img) { return {}; }  // TODO
// Mirror left <-> right.
Image flip_horizontal([[maybe_unused]] const Image& img) { return img; }  // TODO
// Rotate 90 degrees clockwise. Careful: the output shape is (cols x rows).
Image rotate90_cw([[maybe_unused]] const Image& img) { return img; }  // TODO
// Your own filter stage for the Pipeline: implement apply() using threshold().
class ThresholdFilter final : public ImageFilter {
public:
    explicit ThresholdFilter(std::uint8_t t) : t_(t) {}
    Image apply(const Image& in) const override { return in; }    // TODO
    std::string name() const override { return "threshold"; }
private:
    std::uint8_t t_;
};
// Bonus: mean (average) of a FloatImage region — the building block of a box filter.

void exercises() {
    const Image img{{10, 200}, {130, 90}};
    CHECK(threshold(img, 128) == Image({{0, 255}, {255, 0}}));
    auto h = histogram(img);
    CHECK(h[10] == 1 && h[200] == 1 && std::accumulate(h.begin(), h.end(), 0) == 4);
    CHECK(flip_horizontal(img) == Image({{200, 10}, {90, 130}}));
    const Image tall{{1, 2, 3}, {4, 5, 6}};
    CHECK(rotate90_cw(tall) == Image({{4, 1}, {5, 2}, {6, 3}}));
    Pipeline p;
    p.add(std::make_unique<BlurFilter>(3)).add(std::make_unique<ThresholdFilter>(20));
    Image spot(5, 5, 0);
    spot(2, 2) = 255;
    Image mask = p.run(spot);
    CHECK(mask(2, 2) == 255 && mask(1, 1) == 255 && mask(0, 0) == 0);
}

}  // namespace s17

// =====================================================================================
// MAIN — choose sections from the command line:   [section number | all] [ex]
// =====================================================================================
struct Section {
    int id;
    const char* title;
    void (*demo)();
    void (*exercises)();
};

int main(int argc, char* argv[]) {
    const Section sections[] = {
        {1, "Basics: types, variables, const, casts, I/O", s01::demo, s01::exercises},
        {2, "Control flow: if / else, switch, ternary, enum class", s02::demo, s02::exercises},
        {3, "Loops", s03::demo, s03::exercises},
        {4, "Functions and lambdas", s04::demo, s04::exercises},
        {5, "Strings", s05::demo, s05::exercises},
        {6, "Arrays, pointers and references", s06::demo, s06::exercises},
        {7, "Sequence containers: vector, deque, list", s07::demo, s07::exercises},
        {8, "Associative containers: map, unordered_map, set", s08::demo, s08::exercises},
        {9, "Stack, queue, heap, pair, tuple, optional, variant", s09::demo, s09::exercises},
        {10, "Structs and classes (OOP 1)", s10::demo, s10::exercises},
        {11, "Inheritance and polymorphism (OOP 2)", s11::demo, s11::exercises},
        {12, "Operator overloading", s12::demo, s12::exercises},
        {13, "Memory, RAII, smart pointers, move semantics", s13::demo, s13::exercises},
        {14, "Templates", s14::demo, s14::exercises},
        {15, "STL algorithms", s15::demo, s15::exercises},
        {16, "Errors, math and files", s16::demo, s16::exercises},
        {17, "Matrix class and image operations", s17::demo, s17::exercises},
    };

    int only = 0;                  // 0 = run every section
    bool run_exercises = false;
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "ex") run_exercises = true;
        else if (arg != "all") only = std::atoi(arg.c_str());
    }

    for (const Section& s : sections) {
        if (only != 0 && s.id != only) continue;
        std::cout << "\n=== " << std::setw(2) << std::setfill('0') << s.id << std::setfill(' ')
                  << ". " << s.title << " ===\n";
        const int failed_before = g_fail;
        s.demo();
        if (run_exercises) {
            std::cout << "  >> YOUR TURN exercises\n";
            s.exercises();
        }
        if (g_fail != failed_before)
            std::cout << "  (" << g_fail - failed_before << " check(s) failed in this section)\n";
    }
    return report();
}
