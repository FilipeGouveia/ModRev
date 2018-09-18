#ifndef ASPHELPER_H_
#define ASPHELPER_H_

#include <string>
#include <vector>
#include "Network.h"

class ASPHelper {

    public:
        static int checkConsistency(std::string input_file_network, std::vector<std::vector<std::string>> & result, bool ss = true);
        //returns a vector of vectors of solutions
        static std::vector<std::vector<std::string>> getOptAnswer(std::string input, int & optimization, bool optAll = true);
        static void parseNetwork(std::string input_file_network, Network * network);
        static std::vector<FunctionRepairs*> parseFunctionRepairResults(std::vector<std::vector<std::string>> results);


};
#endif
