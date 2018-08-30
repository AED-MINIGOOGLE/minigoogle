#ifndef __PARSER__
#define __PARSER__

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

struct RetrievalData{
    std::string contain;
    std::string file_location;
    std::string title;
    unsigned int db_index;
	RetrievalData(int _db_i, std::string _t, std::string c, std::string f_l):db_index(_db_i), title(_t),contain(c), file_location(f_l){};
};

class Parser {
private:
	std::string File_dir;
	bool getHead(const std::string & s);
	bool get_eo_doc(const std::string & s);
	void get_value_of(const std::string & query, const std::string & line, std::string & val);
	void get_value_of(const std::string & query, const std::string & line, unsigned int & val);
public:
	Parser(std::string f) : File_dir(f) {};
	bool getNextDocument(std::ifstream & file_open, std::vector<RetrievalData> & ans);
	void getDocuments(std::vector<RetrievalData> & ans);
};
bool Parser::getHead(const std::string & s) {
	if (s.size() > 3) {
		if (s.substr(0, 4) == "<doc")
		{
			return true;
		}
	}
}

bool Parser::get_eo_doc(const std::string & s) {
	if (s.substr(0, 6) == "</doc>")
		return true;
	return false;
}
void Parser::get_value_of(const std::string & query, const std::string & line, std::string & val){
	std::size_t found = line.find(query);
	std::size_t start_pos = found + query.size();
	std::size_t found_start_number = line.find('"',start_pos)+1;
	std::size_t found_finish_number = line.find('"',found_start_number);
	val = line.substr(found_start_number,found_finish_number-found_start_number);
}
void Parser::get_value_of(const std::string & query, const std::string & line, unsigned int & val){
	std::string value;
	get_value_of(query,line,value);
	val = std::stol(value);
}

bool Parser::getNextDocument(std::ifstream & file_open, std::vector<RetrievalData> & ans) {
	std::string line;
        do{
            if(!std::getline(file_open, line))
                return false;
                        
        }while(!getHead(line));
        unsigned int dbindex;
        get_value_of("dbindex=",line,dbindex);
        std::string title;
        get_value_of("title=",line,title);
        //Get Id of Document
        /*std::size_t found = line.find("id=");
        std::size_t start_pos = found + 3;
        std::size_t found_start_number = line.find('"',start_pos)+1;
        std::size_t found_finish_number = line.find('"',found_start_number);
        std::string id_str = line.substr(found_start_number,found_finish_number-found_start_number);
        */
        //std::cout<<ccc<<""<<found<<std::endl;
        //Get text of document
        std::string content = "";
        while(true){
            if(!std::getline(file_open, line))
                return false;
            if(!get_eo_doc(line) ){
                if(line.substr(0,12) != "ENDOFARTICLE"){
                    content.append(line);
                    content.append(" ");
                }
            }
            else{
                break;
            }
        }
        //std::cout<< title<<std::endl<<std::endl;
        //std::cout << content <<std::endl<<std::endl;

        ans.push_back(RetrievalData(dbindex, title, content, this->File_dir));
        return true;
}

void Parser::getDocuments(std::vector<RetrievalData> & ans) {
	std::ifstream file_open(File_dir, std::ifstream::binary);
        /*for(int i = 0; i < 20; i++){
            get_nex_Document(cc, file_open, ans);
        }*/
        while(getNextDocument(file_open, ans)){

        }
        file_open.close();
}

#endif
