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
    void search(string const& query, map<int, int>& freqs);
    int num_files();
    int i = 0;
	map<int, RetrievalData*> mDocs;
private:
    void process_file(string& filename);
    FrequencyMap mMap;
    vector<string> mFiles;
};
#endif

