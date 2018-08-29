#ifndef __PARSER__
#define __PARSER__

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

struct retrieval_data{
    std::string contain;
    std::string file_location;
    std::string title;
    unsigned int db_index;
    retrieval_data(int _db_i, std::string _t, std::string c, std::string f_l):db_index(_db_i), title(_t),contain(c), file_location(f_l){};
};

class Parser {
private:
	std::string File_dir;
	bool getHead(const std::string & s, int & c);
	bool get_eo_doc(const std::string & s);
	void get_value_of(const std::string & query, const std::string & line, std::string & val)
	void get_value_of(const std::string & query, const std::string & line, unsigned int & val)
public:
	Parser(std::string f) : File_dir(f) {};
	bool get_nex_Document(std::ifstream & file_open, std::vector<retrieval_data> & ans) 
	void getDocuments(std::vector<RetrievalData> & ans);
};
#endif
