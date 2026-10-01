// Exercise 1.2 — Word frequency + top-k
// Python equivalent: collections.Counter(words).most_common(k)
//
// countWords: split on any non-letter character, lowercase everything.
//   "Edge, edge; EDGE!" -> {"edge": 3}
// topK: return the k most frequent words, sorted by count DESC, ties broken by word ASC.
//   If k > number of words, return all of them.
//
// Hints: std::isalpha / std::tolower (cast the char to unsigned char first!),
//        copy map into std::vector<std::pair<...>>, then std::sort with a lambda.

#include <algorithm>
#include <cctype>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
#include "../check.hpp"

using Counts = std::unordered_map<std::string, int>;

Counts countWords(const std::string& text) {
    Counts counts;
    // TODO
    std::string word;
    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            word += std::tolower(static_cast<unsigned char>(c));
        } else if (!word.empty()) {
            counts[word]++;
            word.clear();
        }
    }
    if (!word.empty()) {
        counts[word]++;
    }
    return counts;
}

std::vector<std::pair<std::string, int>> topK(const Counts& counts, size_t k) {
    std::vector<std::pair<std::string, int>> result(counts.begin(), counts.end());
    std::sort(result.begin(), result.end(), [](const std::pair<std::string, int>& a, const std::pair<std::string, int>& b) {
        if (a.second != b.second) {
            return a.second > b.second; // Sort by count DESC
        }
        return a.first < b.first; // Sort by word ASC
    });
    if (k < result.size()) {
        result.resize(k);
    }
    return result;
}

// ---------------- tests (don't edit) ----------------
int main() {
    auto c = countWords("Edge, edge; EDGE! corner blob corner.");
    CHECK(c.size() == 3);
    CHECK(c.count("edge") && c.at("edge") == 3);
    CHECK(c.count("corner") && c.at("corner") == 2);
    CHECK(c.count("blob") && c.at("blob") == 1);

    auto t = topK(c, 2);
    CHECK(t.size() == 2);
    CHECK(t.size() == 2 && t[0].first == "edge" && t[1].first == "corner");

    auto tie = topK(countWords("b a c"), 10);
    CHECK(tie.size() == 3);
    CHECK(tie.size() == 3 && tie[0].first == "a" && tie[2].first == "c");

    CHECK(countWords("").empty());
    CHECK(countWords("123 !!!").empty());
    return report();
}
