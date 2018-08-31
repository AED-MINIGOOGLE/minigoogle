#ifndef __FREQUENCY_MAP__HPP__
#define __FREQUENCY_MAP__HPP__

#include<map>
#include<vector>
#include<string>

typedef std::vector<std::pair<int, int>> search_result;

struct double_hash {
    uint64_t hash1;
    uint64_t hash2;
    const bool operator<(double_hash const& b) const;
};

class FrequencyMap {
public:
	void search(std::string& query, std::map<int, int>& freqs);
    void insert(std::string& contain, const int key);
private:
	std::string _hash(const char* word);
	std::map<std::string, std::map<int, int>> _map;
};
#endif
