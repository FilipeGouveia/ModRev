#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <string>
#include <map>

class Configuration {

    public:
        static void parseConfig();
        static std::string getValue(std::string key);
        static int getIntValue(std::string key);
        static bool isActive(std::string key);
        static void printConfig();
        static void setValue(std::string key, std::string value);

    private:
        static std::map<std::string,std::string> _configMap;

};

#endif
