
#include<fstream>
#include<cstring>
#include <iostream>
#include "coreengine.hpp"
#include "utility.hpp"
#include "parser.hpp"

using namespace std;

CoreEngine::CoreEngine() {}

CoreEngine::~CoreEngine() {}

int CoreEngine::num_files() {
    return mFiles.size();
}

vector<string> CoreEngine::populate(string const& dirname) {
    vector<string> files = list_immediate_files(dirname);

    mFiles.insert(mFiles.end(), files.begin(), files.end());

	for (auto file : files) {
		std::cout << file << std::endl;
	}

    for (auto iter = mFiles.end() - files.size(); iter != mFiles.end(); iter++) {
        process_file(*iter);
    }
    return files;
}

search_result CoreEngine::search(string const& query) {
    return mMap.search(query);
}

void CoreEngine::process_file(string& filename) {
    ifstream file;
    file.open(filename.c_str(), ifstream::in);

	Parser p(filename);
	std::vector<RetrievalData> docs;
	p.getDocuments(docs);
	for (auto doc : docs) {
		std::cout << doc.key << std::endl;
		std::cout << doc.contain << std::endl;


		int word_pos = 0;
		char word[100];
		memset(word, 0, 100 * sizeof(char));

		for (unsigned i = 0; i < doc.contain.length(); ++i) {
			if (isalpha(doc.contain.at(i))) {
				word[word_pos++] = doc.contain.at(i);
			}
			else {
				if (word_pos > 0) {
					mMap.insert(word, doc.key, &filename);
					word_pos = 0;
					memset(word, 0, 100 * sizeof(char));
				}
			}
		}

		if (word_pos > 0)
			mMap.insert(word, doc.key, &filename);

	}

    file.close();
}
