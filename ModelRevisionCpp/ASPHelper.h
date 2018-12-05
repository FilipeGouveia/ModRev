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
        static std::vector<InconsistencySolution*> parseFunctionRepairResults(std::vector<std::vector<std::string>> results);
        static std::vector<Function*> getFunctionReplace(Function* function, bool is_fathers);
        static std::string constructFunctionClause(Function* function);
        static std::vector<Function*> parseFunctionFamily(std::string input, Function* original);


};
#endif
