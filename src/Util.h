
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

std::string getFilename(const std::string &s);
std::string getFilepath(const std::string &s);
bool validateInputName(const std::string &s);
std::string getOutputName(const std::string &s);
std::string escapeString(const std::string &s);

#endif /* Util_h */
