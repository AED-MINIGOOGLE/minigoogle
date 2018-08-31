#ifndef __COREENGINE__HPP__
#define __COREENGINE__HPP__

#include<string>
#include<vector>
#include<map>
#include "frequencymap.hpp"
#include "parser.hpp"

class CoreEngine {
public:
    CoreEngine();
    ~CoreEngine();
    std::vector<std::string> populate(std::string const& dirname);
    void search(std::string const& query, std::map<int, int>& freqs);
    int num_files();
    int i = 0;
	std::map<int, RetrievalData*> mDocs;
private:
    void process_file(std::string& filename);
    FrequencyMap mMap;
    std::vector<std::string> mFiles;
};
#endif

