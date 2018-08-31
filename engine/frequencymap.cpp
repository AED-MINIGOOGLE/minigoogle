#include<cstring>
#include "frequencymap.hpp"

void FrequencyMap::search(const std::string& query, std::map<int, int>& freqs) {
   
	freqs = _map[query];

}

void FrequencyMap::insert(const char * word, const int key) {
    _map[_hash2(word)][key]++;
}

std::string FrequencyMap::_hash2(const char* word) {
	return std::string(word);
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

