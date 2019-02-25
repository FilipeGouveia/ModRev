#ifndef ASPHELPER_H_
#define ASPHELPER_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "Network.h"

class ASPHelper {

    public:
        static void parseNetwork(std::string input_file_network, Network * network);
        static std::vector<Function*> getFunctionReplace(Function* function, bool is_fathers);
        static std::string constructFunctionClause(Function* function);
        static std::vector<Function*> parseFunctionFamily(std::string input, Function* original);


};
#endif
