#include "Configuration.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

std::map<std::string,std::string> Configuration::_configMap;

void Configuration::parseConfig(std::string filename) {

    //default known values
    _configMap["check_ASP"] = "false"; //use ASP consistency check program
    _configMap["function_ASP"] = "true"; //use ASP function program
    _configMap["ASP_dir"] = "../ASP/"; //ASP disrectory
    _configMap["ASP_solver"] = "./../ASP/bin-Darwin/clingo"; // ASP solver
    _configMap["ASP_CC_BASE"] =  "../ASP/base.lp"; //ASP consistency check base program file
    _configMap["ASP_CC_SS"] =  "../ASP/StableState/core-ss.lp"; //ASP consistency check steady state program file
    _configMap["ASP_CC_D"] =  "../ASP/Dynamic/core-ts.lp"; //ASP consistency check dynamic program file
    _configMap["ASP_CC_D_A"] =  "../ASP/Dynamic/a-update.lp"; //ASP consistency check dynamic asyncronous update program file
    _configMap["ASP_CC_D_S"] =  "../ASP/Dynamic/s-update.lp"; //ASP consistency check dynamic synchronous update program file
    _configMap["ASP_CC_D_MA"] =  "../ASP/Dynamic/ma-update.lp"; //ASP consistency check dynamic multi-asynchronous update program file
    _configMap["ASP_Functions"] = "../ASP/Functions/"; // ASP function files directory
    _configMap["allOpt"] = "true"; // show one or more solutions
    _configMap["debug"] = "false";
    _configMap["multipleProfiles"] = "true";
    _configMap["compareLevelFunction"] = "true";
    _configMap["exactMiddleFunctionDetermination"] = "true";
    _configMap["ignoreWarnings"] = "false";
    _configMap["forceOptimum"] = "false";
    _configMap["showSolutionForEachInconsistency"]= "false"; //show best solution for each consistency check solution even if it is not globally optimum
    _configMap["checkConsistency"] = "false"; //just check the consistency of the model and return

    //read new values from config file

    std::ifstream f_input(filename);

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

void Configuration::setValue(std::string key, std::string value){
    _configMap[key] = value;
    return;
}