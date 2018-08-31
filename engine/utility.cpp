#include "utility.hpp"
#include<chrono>
#include<cerrno>
#include<dirent.h>

using namespace std::chrono;

double profile(function<void(void)> func) {
    auto start = high_resolution_clock::now();
    func();
    auto finish = high_resolution_clock::now();
	return duration_cast<nanoseconds>(finish - start).count() / 10e3;
}

vector<string> list_immediate_files(string const& dirname) {
    vector<string> filenames;
    DIR *target = opendir(dirname.c_str());
    if (target != NULL) {
        for (dirent *entry = readdir(target); 
             entry != NULL ;
             entry = readdir(target)) {
            if (entry->d_name[0] == '.' ||
                (entry->d_type & DT_DIR)) continue;
            filenames.push_back(dirname + "/" + string(entry->d_name));
        }
        closedir(target);
    }
    return filenames;
}

bool specialChar(char& c) {

	switch (c)
	{
		case 'á': 
		case 'Á':
		{
			c = 'a';
			return true;
		}
		case 'é': 
		case 'É':
		{
			c = 'e';
			return true;
		}
		case 'í':
		case 'Í':
		{
			c = 'i';
			return true;
		}
		case 'ó':
		case 'Ó':
		{
			c = 'o';
			return true;
		}
		case 'ú':
		case 'Ú':
		{
			c = 'u';
			return true;
		}
		case 'ñ':
		case 'Ñ': 
		{
			c = 'ñ';
			return true;
		}
	}

	return false;
}
