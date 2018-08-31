
#include<cstring>
#include "coreengine.hpp"
#include "utility.hpp"


CoreEngine::CoreEngine() {}

CoreEngine::~CoreEngine() {}

int CoreEngine::num_files() {
    return mFiles.size();
}

std::vector<std::string> CoreEngine::populate(std::string const& dirname) {
    std::vector<std::string> files = list_immediate_files(dirname);

    mFiles.insert(mFiles.end(), files.begin(), files.end());

    for (auto iter = mFiles.end() - files.size(); iter != mFiles.end(); iter++) {
        process_file(*iter);
		this->i++;
    }
    return files;
}

void CoreEngine::search(std::string const& query, std::map<int, int>& freqs) {
    mMap.search(query, freqs);
}

void CoreEngine::process_file(std::string& filename) {

	Parser p(filename);
	std::vector<RetrievalData> docs;
	p.getDocuments(docs);
	for (auto doc : docs) {
		
		mDocs[doc.db_index] = new RetrievalData(doc.db_index, doc.title, doc.contain, doc.file_location);

		int word_pos = 0;
		char word[147];
		memset(word, 0, 147 * sizeof(char));

		for (unsigned i = 0; i < doc.contain.length(); ++i) {
			if (isalpha(doc.contain.at(i)) || specialChar(doc.contain.at(i))) {
				word[word_pos++] = doc.contain.at(i);
			}
			else {
				if (word_pos > 0) {
					for (int i = 0; i < word_pos; ++i)
						word[i] = tolower(word[i]);

					mMap.insert(word, doc.db_index);
					word_pos = 0;
					memset(word, 0, 147 * sizeof(char));
				}
			}
		}

		if (word_pos > 0)
			mMap.insert(word, doc.db_index);

	}

}
