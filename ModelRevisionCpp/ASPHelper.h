#ifndef ASPHELPER_H_
#define ASPHELPER_H_

#include <string>
#include <vector>
#include "Network.h"
#include <clingo.hh>

class ASPHelper {

    public:
        //parses the input file of a network model in ASP format
        static int parseNetwork(Network * network);

        //check the consistency of a model and returns a vector of minimum inconsistency solutions
        static std::vector<InconsistencySolution*> checkConsistency(Network * network, int & optimization, bool ss = false, int update = 0);
        

        //static std::vector<std::vector<std::string>> getOptAnswer(std::string input, int & optimization, bool optAll = true);
        //static std::vector<InconsistencySolution*> parseFunctionRepairResults(std::vector<std::vector<std::string>> results);
        //static std::vector<Function*> getFunctionReplace(Function* function, bool is_fathers, std::string filename = "");
        //static std::string constructFunctionClause(Function* function);
        //static std::vector<Function*> parseFunctionFamily(std::string input, Function* original);

    private:
        //parses a clingo model of the consistency check into an InconsistencySolution object
        static InconsistencySolution * parseCCModel(const Clingo::Model &m, int & optimization);


};
#endif
