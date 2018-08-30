#ifndef __FREQUENCY_MAP__HPP__
#define __FREQUENCY_MAP__HPP__
#include<cstdint>
#include<unordered_map>
#include<map>
#include<vector>
#include<algorithm>
#include<cstring>
#include <fstream>

using namespace std;

typedef vector<pair<map<int, int>, string*>> search_result;

struct double_hash {
    uint64_t hash1;
    uint64_t hash2;
    const bool operator<(double_hash const& b) const;
};

class FrequencyMap {
public:
    search_result search(string const& query);
    void insert(char * const word, const int key, string * const index);
private:
    const double_hash _hash(const char* const input) const;

    map<double_hash, map<string*, map<int, int>>> _map;
};

search_result FrequencyMap::search(string const& query) {
    auto freqs = _map[_hash(query.c_str())];
    search_result result;
    for (auto& it : freqs) {
        result.push_back(make_pair(it.second, it.first));
    }
    stable_sort(result.rbegin(), result.rend());
    return result;
}

void FrequencyMap::insert(char * const word, const int key, string * const index) {
    word[0] = tolower(word[0]);
    _map[_hash(word)][index][key]++;
}

const double_hash FrequencyMap::_hash(const char * const input) const {
    uint64_t h2 = 0;
    union { uint64_t h1; uint64_t u[8]; };
    const char* str = input;
    h1 = strlen(str);
    for (int i = 0; *str; i++, str++) {
        u[i % 8] += *str + i + (*str >> ((h1 / (i + 1)) % 5));
        h2 += *str;
    }
    return {h1, h2};
}

const bool double_hash::operator<(double_hash const& b) const {
    if (this->hash1 < b.hash1) return true;
    if (this->hash1 > b.hash1) return false;
    if (this->hash2 < b.hash2) return true;
    if (this->hash2 > b.hash2) return false;
    return false;
};
#endif
