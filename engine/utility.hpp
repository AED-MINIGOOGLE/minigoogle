#ifndef __UTILITY__HPP__
#define __UTILITY__HPP__
#include<vector>
#include<functional>
#include<string>

double profile(std::function<void(void)> func);
std::vector<std::string> list_immediate_files(std::string const& dirname);

bool specialChar(char& c);
#endif
