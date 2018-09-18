
#ifndef Util_h
#define Util_h

#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <vector>

void split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

std::string exec(const char* cmd);

#endif /* Util_h */
