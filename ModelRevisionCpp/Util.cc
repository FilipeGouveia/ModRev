
#include "Util.h"
#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>


void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}



std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}

std::string getFilename(const std::string &s) 
{
    if(s.empty())
        return "";
    std::string filename = s;
    std::vector<std::string> elems;
    elems = split(filename, '/');
    filename = elems.back();
    elems = split(filename, '.');
    filename = "";
    if(elems.size() > 1)
    {
        for(int i = 0; i < (int) elems.size() - 1; i++)
        {
            filename.append(elems[i]);
        }
    }
    else
    {
        filename.append(elems[0]);
    }
    return filename;
}

std::string getFilepath(const std::string &s) 
{
    if(s.empty())
        return "./";
    std::string filepath = s;
    std::vector<std::string> elems;
    elems = split(filepath, '/');
    filepath = "";
    if(elems.size() > 1)
    {
        for(int i = 0; i < (int) elems.size() - 1; i++)
        {
            filepath.append(elems[i]);
            filepath.append("/");
        }
    }
    else
    {
        return "./";
    }
    return filepath;
}