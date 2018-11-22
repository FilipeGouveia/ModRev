#include "Configuration.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::map<std::string,std::string> Configuration::_configMap;

void Configuration::parseConfig() {

    //default known values
    _configMap["check_ASP"] = "false";
    _configMap["function_ASP"] = "true";
    _configMap["ASP_dir"] = "../ASP/";
    _configMap["ASP_solver"] = "./../ASP/bin-Darwin/clingo";
    _configMap["ASP_CC_SS"] =  "../ASP/ConsistencyCheck/core-ss.lp";
    _configMap["ASP_Functions"] = "../ASP/Functions/";

    //read new values from config file

    std::ifstream f_input("config.txt");

    if(f_input.is_open())
    {
        std::string line;
        while(std::getline(f_input,line))
        {
            std::istringstream f_line(line);
            std::string key;
            if(std::getline(f_line, key, '='))
            {
                std::string value;
                if(std::getline(f_line, value))
                {
                    _configMap[key] = value;
                }

            }
        }
        f_input.close();
    }
    
}

std::string Configuration::getValue(std::string key)
{
    auto it = _configMap.find(key);
    if (it != _configMap.end())
    {
        return it->second;
    }

    return "";
}

int Configuration::getIntValue(std::string key)
{
    auto it = _configMap.find(key);
    if (it != _configMap.end())
    {
        try{
            int value = std::stoi(it->second);
            return value;
        }
        catch(std::exception& e)
        {
            return -1;
        }
    }
    return -1;
}

bool Configuration::isActive(std::string key)
{
    auto it = _configMap.find(key);
    if (it != _configMap.end())
    {
        if(it->second == "true" || it->second == "True" || it->second == "T" || it->second == "1")
        {
            return true;
        }
        if(it->second == "false" || it->second == "False" || it->second == "F" || it->second == "0")
        {
            return false;
        }
        try{
            int value = std::stoi(it->second);
            return value > 0;
        }
        catch(std::exception& e)
        {
            return false;
        }
    }
    return false;
}

void Configuration::printConfig()
{
    for(auto it = _configMap.begin(), end = _configMap.end(); it!=end; it++)
    {
        std::cout << it->first << " --> " << it->second << std::endl;
    }

}