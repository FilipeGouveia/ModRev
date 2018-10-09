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

    //main function that revises the model
    modelRevision(input_file_network);
    
};


// Function that initializes the program.
// Can process optional arguments or configurations
void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file) {

    input_file_network = argv[1];

    if(argc > 2)
        output_file = argv[2];

    return;
};



//Model revision procedure
// 1) tries to repair functions
// 2) tries to flip the sign of the edges
// 3) tries to add or remove edges
void modelRevision(std::string input_file_network) {

    bool is_consistent = false;
    int optimization = -2;

    std::vector<FunctionInconsistencies*> fInconsistencies = checkConsistencyFunc(input_file_network, optimization);

    //At this point we have an inconsistent network with functions candidates to be repaired

    //for each possible inconsistency solution, try to make the model consistent
    std::vector<Function*> newFunctions; 
    for(auto it = fInconsistencies.begin(), end = fInconsistencies.end(); it != end; it++)
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


//function reponsible to check the consistency of a model and return a set of possible function inconsistencies
std::vector<FunctionInconsistencies*> checkConsistencyFunc(std::string input_file_network, int & optimization) {

    std::vector<std::vector<std::string>> result_raw;
    std::vector<FunctionInconsistencies*> result;
    
    //consistency check
    if(Configuration::check_ASP)
    {
        // invoke the consistency check program in ASP
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
        //parse the raw results to an internal representation.
        //this should be done at ASP level in the check consistency function
        //TODO
        result = ASPHelper::parseFunctionRepairResults(result_raw);
    }

    return result;
};



//This function receives an inconsistent model and a set of functions to be repaired and try to repair the target functions making the model consistent
//returns the set of new functions to be replaced
std::vector<Function*> repairFuncConsistency(std::string input_file_network, FunctionInconsistencies* repairSet, int & optimization){

    std::vector<Function*> result;
    std::vector<std::vector<Function*>> candidates;


    if(Configuration::function_ASP)
    {
        for(auto it = repairSet->generalization_.begin(), end = repairSet->generalization_.end(); it != end; it++)
        {
            //TODO each function must have a list of replacement candidates and each msut be tested until it works
            Function* originalF = network->getNode((*it))->getFunction();
            if(originalF == nullptr)
            {
                continue;
            }


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


