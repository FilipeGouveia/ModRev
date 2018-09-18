#include "Configuration.h"
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

bool Configuration::check_ASP = true;
bool Configuration::function_ASP = true;
std::string Configuration::ASP_dir = "../ASP/";
std::string Configuration::ASP_solver = "./../ASP/bin-Darwin/clingo";
std::string Configuration::ASP_CC_SS = "../ASP/ConsistencyCheck/core-ss.lp";

void Configuration::parseConfig() {

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
                    _store_config(key, value);

            }
        }
        f_input.close();
    }
    
};

void Configuration::_store_config(std::string key, std::string value) {

    if(key == "check_ASP")
    {
        if(value == "true")
            check_ASP = true;
        else
            if(value == "false")
                check_ASP = false;
        return;
    }
    if(key == "function_ASP")
    {
        if(value == "true")
            function_ASP = true;
        else
            if(value == "false")
                function_ASP = false;
        return;
    }
    if(key == "ASP_dir")
    {
        ASP_dir = value;
        return;
    }
    return;
};

