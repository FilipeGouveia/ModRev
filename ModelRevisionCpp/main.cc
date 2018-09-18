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
    std::vector<std::vector<std::string>> result;

    //consistency check
    if(Configuration::check_ASP)
    {

        optimization = ASPHelper::checkConsistency(input_file_network, result);
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
        return;
    }

    if(optimization == 0)
    {
        std::cout << "This network is consistent!" << std::endl;
        return;
    }

    if(optimization > 0)
    {
        ASPHelper::parseNetwork(input_file_network, network);

        std::vector< Node* > nodes = network->getNodes();
        for(auto it = nodes.begin(), end = nodes.end(); it != end; it++)
        {
            std::cout << " " << (*it)->id_ << std::endl;
        }
    }


    //At this point we have an inconsistent network with possible repairs

    std::vector<FunctionRepairs*> fRepairs = ASPHelper::parseFunctionRepairResults(result);


};
