#include "main.h"
#include "Configuration.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ASPHelper.h"
#include "Network.h"

Network * network = new Network();

int main(int argc, char ** argv) {

    Configuration::parseConfig();

    std::string input_file_network, output_file;

    if(argc < 2)
    {
        std::cout << "Invalid number of arguments: " << argc << std::endl;
        return -1;
    }

    process_arguments(argc, argv, input_file_network, output_file);

    ASPHelper::parseNetwork(input_file_network, network);
    modelRevision(input_file_network);
    
};


void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file) {

    input_file_network = argv[1];

    if(argc > 2)
        output_file = argv[2];

    return;
};



void modelRevision(std::string input_file_network) {

    bool is_consistent = false;
    int optimization = -2;

    //At this point we have an inconsistent network with possible repairs
    std::vector<FunctionRepairs*> fRepairs = checkConsistencyFunc(input_file_network, optimization);


    //for each possible repair solution, try to make the model consistent
    std::vector<Function*> newFunctions; 
    for(auto it = fRepairs.begin(), end = fRepairs.end(); it != end; it++)
    {
        if(optimization != 0)
        {
            newFunctions = repairFuncConsistency(input_file_network, (*it), optimization);
        }
        if(optimization == 0)
        {
            break;
        }
    }

};



std::vector<FunctionRepairs*> checkConsistencyFunc(std::string input_file_network, int & optimization) {

    std::vector<std::vector<std::string>> result_raw;
    std::vector<FunctionRepairs*> result;
    
    //consistency check
    if(Configuration::check_ASP)
    {

        optimization = ASPHelper::checkConsistency(input_file_network, result_raw);
    }
    else
    {
        //TODO Add other implementations
        //convert ASP to sat or other representation
        //test consistency
    }

    //TODO Consider repair edges
    if(optimization < 0)
    {
        std::cout << "It is not possible to repair this network for now." << std::endl;
    }

    if(optimization == 0)
    {
        std::cout << "This network is consistent!" << std::endl;
    }

    if(optimization > 0 && Configuration::check_ASP)
    {
        result = ASPHelper::parseFunctionRepairResults(result_raw);
    }

    return result;
};



std::vector<Function*> repairFuncConsistency(std::string input_file_network, FunctionRepairs* repairSet, int & optimization){

    std::vector<Function*> result;
    std::vector<std::vector<Function*>> candidates;

    if(Configuration::function_ASP)
    {
        for(auto it = repairSet->generalization_.begin(), end = repairSet->generalization_.end(); it != end; it++)
        {
            //TODO each function must have a list of replacement candidates and each msut be tested until it works
            Function* originalF = network->getNode((*it))->getFunction();
            //if the function only has 1 regulator then it is not possible to change the function
            //better try to flip the sign of the edge
            if(originalF->getNumberOfRegulators() < 2)
            {
                std::cout << "WARN! Not possible to repair function of " << (*it) << std::endl;
                return result;
            }
            std::vector<Function*> tCandidates;
            std::vector<Function*> aux = ASPHelper::getFunctionReplace(network->getNode((*it))->getFunction(),true);
            
        }

        
    }
    else
    {
    //TODO support other solvers
    } 

    return result;
};


