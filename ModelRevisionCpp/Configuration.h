#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>

class Configuration {

    public:
        static bool check_ASP;
        static bool function_ASP;
        static std::string ASP_dir;
        static std::string ASP_solver;
        static std::string ASP_CC_SS;
        static std::string ASP_Functions;

        static void parseConfig();

    private:
        static void _store_config(std::string key, std::string value);

};

#endif
