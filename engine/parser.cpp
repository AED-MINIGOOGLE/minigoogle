
#include "parser.hpp"

bool Parser::getHead(const std::string & s, int & c) {
	if (s.size() > 3) {
		if (s.substr(0, 4) == "<doc")
		{
			c++;
			return true;
		}
	}
}

bool Parser::get_eo_doc(const std::string & s) {
	if (s.substr(0, 6) == "</doc>")
		return true;
	return false;
}

bool Parser::getNexDocument(int & counter, std::ifstream & file_open, std::vector<RetrievalData> & ans) {
	//std::ifstream file_open(File_dir, std::ifstream::binary);
	std::string line;
	int ccc = 0;
	do {
		if (!std::getline(file_open, line))
			return false;

	} while (!getHead(line, ccc));
	//Get Id of Document
	std::size_t found = line.find("id=");
	std::size_t start_pos = found + 3;
	std::size_t found_start_number = line.find('"', start_pos) + 1;
	std::size_t found_finish_number = line.find('"', found_start_number);
	std::string id_str = line.substr(found_start_number, found_finish_number - found_start_number);
	//std::cout<<ccc<<""<<found<<std::endl;
	//Get text of document
	std::string content = "";
	while (true) {
		if (!std::getline(file_open, line))
			return false;
		if (!get_eo_doc(line)) {
			if (line.substr(0, 12) != "ENDOFARTICLE" && line != "") {
				content.append(line);
			}
		}
		else {
			break;
		}
	}
	//std::getline(file_open,line);
	//file_open.close();
	ans.push_back(RetrievalData(std::stol(id_str), content, this->File_dir));
	return true;
}

void Parser::getDocuments(std::vector<RetrievalData> & ans) {
	std::ifstream file_open(File_dir, std::ifstream::binary);
	int cc;
	while (getNexDocument(cc, file_open, ans)) {

	}
	file_open.close();
}