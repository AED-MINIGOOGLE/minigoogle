#ifndef __COREENGINE__HPP__
#define __COREENGINE__HPP__

#include<string>
#include<vector>
#include<map>
#include "frequencymap.hpp"
#include "parser.hpp"

using namespace std;

class CoreEngine {
public:
    CoreEngine();
    ~CoreEngine();
    vector<string> populate(string const& dirname);
    search_result search(string const& query);
    int num_files();
	map<int, RetrievalData*> mDocs;
private:
    void process_file(string& filename);
    FrequencyMap mMap;
    vector<string> mFiles;
};
#endif

