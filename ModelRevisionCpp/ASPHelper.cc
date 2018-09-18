#include "ASPHelper.h"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "Util.h"
#include <cstdio>
#include <cstring>
#include "Configuration.h"
#include "Network.h"
#include <fstream>

int ASPHelper::checkConsistency(std::string input_file_network, std::vector<std::vector<std::string>> & result, bool ss) {

    std::string solve_cmd = Configuration::ASP_solver;
    solve_cmd.append(" ");
    if(ss)
        solve_cmd.append(Configuration::ASP_CC_SS);
    solve_cmd.append(" ");
    solve_cmd.append("--opt-mode=optN");
    solve_cmd.append(" ");
    solve_cmd.append(input_file_network);

    std::string result_cmd = exec(solve_cmd.c_str());
    int opt = -2;
    result = ASPHelper::getOptAnswer(result_cmd, opt);
   if(opt == -1)
   {
       std::cout << "found no solution" << std::endl;
       return opt;
   } 
   if(opt == 0)
   {
        std::cout << "model consistent" << std::endl;
        return opt;
   }
   if(opt > 0)
   {
       std::cout << "found " << result.size() << " solutions with " << opt << " repairs" << std::endl;
   }
   return opt;

};


std::vector<std::vector<std::string>> ASPHelper::getOptAnswer(std::string input, int & optimization, bool optAll) {
    
    std::vector<std::vector<std::string>> result;

    int found = 0;
    bool process = false;
    bool to_ignore = false;
    
    std::istringstream iss(input);
    std::string line;
    while(std::getline(iss,line))
    {
        if(line.find("UNSATISFIABLE\n") != std::string::npos)
        {
            optimization = -1;
            break;
        }
        if(line.find("Answer: ") != std::string::npos)
        {
            if(found > 1)
            {
                process = true;
            }
            else 
            {
                if(0 == line.compare(line.length()-2,2," 1"))
                {
                    found++;
                    if(found > 1)
                        process = true;
                }
            }
        }
        else
        {
            if(process && !to_ignore)
            {
                process = false;
                if(!optAll)
                    to_ignore = true;
                result.push_back(Util_h::split(line, ' '));
            }
            else
            {
                if(line.find("Optimization : ") != std::string::npos)
                {
                    try{
                        optimization = std::stoi(Util_h::split(line, ' ').back());
                    }
                    catch(std::exception& e)
                    { }
                }
            }
        }
    }

    return result;

};


void ASPHelper::parseNetwork(std::string input_file_network, Network * network) {

    std::ifstream file;
    file.open(input_file_network);
    if(!file.good())
    {
        std::fprintf(stderr, "ERROR! Cannot open file '%s'.\n", input_file_network.c_str());
        return;
    }

    std::string line;
    while(std::getline(file,line))
    {
        line.erase(remove_if(line.begin(),line.end(),isspace),line.end());
        if(line.find(").") != std::string::npos)
        {
            std::vector<std::string> split = Util_h::split(line,'(');           if(split[0].compare("edge") == 0)
            {
                split = Util_h::split(split[1], ')');
                split = Util_h::split(split[0], ',');
                if(split.size() != 3)
                {
                    std::cout << "WARN! Edge not recognised: " << line << std::endl;
                    continue;
                }
                std::string startId = split[0];
                std::string endId = split[1];
                int sign;
                try {
                    sign = std::stoi(split[2]);
                }
                catch(...)
                {
                    std::cout << "WARN! Invalid edge sign: " << split[2] << " on line " << line << std::endl;
                    continue;
                }
                
                Node* startNode = network->addNode(startId);
                Node* endNode = network->addNode(endId);
                network->addEdge(startNode, endNode, sign);
                continue;
            }
            if(split[0].compare("functionOr") == 0)
            {
                split = Util_h::split(split[1], ')');
                split = Util_h::split(split[0], ',');
                if(split.size() != 2)
                {

                    std::cout << "WARN! FunctionOr not recognised: " << line << std::endl;
                    continue;
                }
                Node* node = network->addNode(split[0]);
                if(split[1].find("..") != std::string::npos)
                {
                    std::vector<std::string> aux_split = Util_h::split(split[1], '.');
                    split[1] = aux_split[aux_split.size()-1];
                }

                int nClauses;
                try {
                    nClauses = std::stoi(split[1]);
                }
                catch(...)
                {
                    std::cout << "WARN! Invalid number of clauses: " << split[1] << " on line " << line << std::endl;
                    continue;
                }
                Function* f = new Function(split[0], nClauses);
                node->addFunction(f);
                continue;
            }
            if(split[0].compare("functionAnd") == 0)
            {
                split = Util_h::split(split[1], ')');
                split = Util_h::split(split[0], ',');
                if(split.size() != 3)
                {
                    std::cout << "WARN! FunctionAnd not recognised: " << line << std::endl;
                    continue;
                }
                Node * node = network->getNode(split[0]);
                if(node == nullptr)
                {
                    std::cout << "WARN! node not recognised: " << split[0] << " on line " << line <<std::endl;
                    continue;
                }
                int clauseId;
                try {
                    clauseId = std::stoi(split[1]);
                }
                catch(...)
                {
                    std::cout << "WARN! Invalid clause Id: " << split[1] << " on line " << line << std::endl;
                    continue;
                }
                node->getFunction()->addElementClause(clauseId, split[2]);
                continue;
            }

        }
    }


};


std::vector<FunctionRepairs*> ASPHelper::parseFunctionRepairResults(std::vector<std::vector<std::string>> results) {
    std::vector<FunctionRepairs*> result;   
    for(auto it = results.begin(), end= results.end(); it!=end; it++)
    {
        FunctionRepairs* repair = new FunctionRepairs();
        for(auto it2 = (*it).begin(), end2 = (*it).end(); it2 != end2; it2++)
        {
            std::vector<std::string> split = Util_h::split(*it2, '(');
            if(split[0].compare("vlabel") == 0)
            {
                split = Util_h::split(split[1], ')');
                split = Util_h::split(split[0], ',');
                int value;
                try
                {
                    value = std::stoi(split[1]);
                }
                catch(...)
                {
                    std::cout << "WARN! Invalid value in label: " << split[1] << std::endl;
                    continue;
                }
                repair->addVLabel(split[0], value);
                continue;
            }
            
            if(split[0].compare("r_gen") == 0)
            {
                split = Util_h::split(split[1], ')');
                repair->addGeneralization(split[0]);
                continue;
            }

            if(split[0].compare("r_part") == 0)
            {
                split = Util_h::split(split[1], ')');
                repair->addParticularization(split[0]);
                continue;
            }

        }

        result.push_back(repair);
    }

    return result;
};
