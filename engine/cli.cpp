#include "cli.hpp"
#include "utility.hpp"
#include<iostream>
#include<vector>
#include<algorithm>

CoreEngine CliApp::getEngine(){
	return this->engine;
}
void CliApp::print(search_result result, double time) {

	int count = 0;
    if (result.size() <= 0) {
        cout << "Not found." << endl;
    } else {
		std::cout << "About " << result.size() << " results (" << time << " ms)" << std::endl;
        for (auto &it : result) {
			std::cout << "[" << count <<"] " << (*(engine.mDocs[it.second])).title << ": " << it.first << std::endl;
			count++;

			if (count % 20 == 0 && result.size()>count) {
				string answer;
				std::cout << "Press (n) for show nex documents, (number) for show the document complete or (q) for nex query" << std::endl;
				cin >> answer;
				std::cout << "\033[2J";

				if (answer == "n")
					continue;
				else if (answer == "q")
					return;
				else {

					try {
						int id = std::stoi(answer, nullptr);
						int db_index = result[id].second;
						std::cout << "\033[2J";
						std::cout << (*(engine.mDocs[db_index])).title << std::endl;
						std::cout << (*(engine.mDocs[db_index])).contain << std::endl;
						return;
					}
					catch (const std::invalid_argument& ia) {
						std::cout << "\033[1J";
						std::cerr << "Invalid argument: " << answer << std::endl;
						return;
					}
				}

			}
        }

		string answer;
		std::cout << "Press (number) for show the document complete or (q) for nex query" << std::endl;
		cin >> answer;
		std::cout << "\033[2J";

		if (answer == "q")
			return;
		else {
			try {
				int db_index = result[std::stoi(answer, nullptr)].second;
				std::cout << "\033[2J";
				std::cout << (*(engine.mDocs[db_index])).title << std::endl;
				std::cout << (*(engine.mDocs[db_index])).contain << std::endl;
				return;
			}
			catch (const std::invalid_argument& ia) {
				std::cout << "\033[2J";
				std::cerr << "Invalid argument: " << answer << std::endl;
				return;
			}
		}
    }
}

int CliApp::run() {
    string dirname = get_dir();
    auto process = [this, &dirname]() {
        this->engine.populate(dirname);
    };

	double time = profile(process);
    cout << "\n... Loading index done! \nTook " << time << " ms to build search index." << endl;
    string query;
    search_result result;
    
	cout << "\n**Enter :q to stop at anytime.**" << endl;
    cout << "Enter search query: ";
    cin >> query;

    while (query != ":q") {
		auto query_index = [this, &query, &result]() {
			result = this->engine.search(query);
		};

		print(result, profile(query_index));

		cout << "**Enter :q to stop at anytime.**" << endl;
		cout << "Enter search query: ";
		cin >> query;
	}

    cout << "Exiting search engine...Done!" << endl;
    return 0;
}

void CliApp::RunWeb(){
    string dirname = get_dir();
    vector<string> res;
    auto process = [this, &dirname,&res]() {
        res = this->engine.populate(dirname);
    };
    cout << res.size() << endl;
    cout << "**Took " << profile(process) << "us to build search index." << endl;

}

search_result CliApp::SearchWeb(string query) {
    search_result result;
        cout << this->engine.i << endl;
        auto query_index = [this, &query, &result] () {
            result = this->engine.search(query);
        };
        cout << "**Took " << profile(query_index) << "us to query tree." << endl;  
		//this->print(result,profile(query_index));  
    return result;
}
int CliApp::prueba(int n){
    return n;
}
CliApp::CliApp(int argc, char **argv) {}
CliApp::~CliApp() {}
CliApp::CliApp() {}
string CliApp::get_dir() {
    string dirname;
    cout << "Enter directory: ";
    getline(cin, dirname);
    cout << "Processing directory at " << dirname << endl;
    return dirname;
}
