#ifndef __PARSER__
#define __PARSER__

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

struct RetrievalData {
	std::string contain;
	std::string file_location;
	int key;
	RetrievalData(int _k, std::string c, std::string f_l) :key(_k), contain(c), file_location(f_l) {};
};

class Parser {
private:
	std::string File_dir;
	bool getHead(const std::string & s, int & c);
	bool get_eo_doc(const std::string & s);
public:
	Parser(std::string f) : File_dir(f) {};
	bool getNexDocument(int & counter, std::ifstream & file_open, std::vector<RetrievalData> & ans); 
	void getDocuments(std::vector<RetrievalData> & ans);
};
#endif