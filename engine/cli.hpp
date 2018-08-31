#ifndef __CLI__HPP__
#define __CLI__HPP__
#include "coreengine.hpp"
#include "frequencymap.hpp"
#include<string>

using namespace std;

class CliApp {
public:
    CliApp();
    CliApp(int argc, char** argv);
    ~CliApp();
    int run();
    search_result SearchWeb(string);
	void sortMap(const map<int, int>& freqs, search_result& result);
    int prueba(int);
    void RunWeb();
private:
    string get_dir();
    void print(const map<int, int>& freqs, const double time);
    vector<string> files;
    CoreEngine engine;
};
#endif
