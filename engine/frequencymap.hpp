#ifndef __FREQUENCY_MAP__HPP__
#define __FREQUENCY_MAP__HPP__
#include<cstdint>
#include<unordered_map>
#include<map>
#include<vector>
#include<string>
using namespace std;

typedef vector<pair<int, int>> search_result;

struct double_hash {
    uint64_t hash1;
    uint64_t hash2;
    const bool operator<(double_hash const& b) const;
};

class FrequencyMap {
public:
	void search(string const& query, map<int, int>& freqs);
    void insert(char * const word, const int key);
private:
    const double_hash _hash(const char* input) const;
	std::string _hash2(const char* word);
    map<string, map<int, int>> _map;
};
#endif
