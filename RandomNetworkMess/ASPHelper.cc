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
#include <bitset>
#include <algorithm>
#include <time.h>

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
        line.erase(std::remove_if(line.begin(),line.end(),isspace),line.end());
        if(line.find(").") != std::string::npos)
        {
            std::vector<std::string> split = Util_h::split(line,'(');
            if(split[0].compare("edge") == 0)
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
            if(split[0].compare("fixed") == 0)
            {
                split = Util_h::split(split[1], ')');
                split = Util_h::split(split[0], ',');
                if(split.size() == 1)
                {
                    std::string node = split[0];
                    Node * n = network->getNode(node);
                    if(n != nullptr)
                    {
                        n->fixed_ = true;
                    }
                    continue;
                }
                if(split.size() != 2)
                {
                    continue;
                }
                std::string startId = split[0];
                std::string endId = split[1];

                Edge* e = network->getEdge(startId, endId);
                if(e != nullptr)
                {
                    e->setFixed();
                }
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

}

std::vector<Function*> ASPHelper::getFunctionReplace(Function* function, bool is_fathers) {
    std::vector<Function*> result;

    std::string function_cmd = Configuration::getValue("ASP_solver");
    function_cmd.append(" ");
    function_cmd.append("0");
    function_cmd.append(" ");
    std::string functions_folder = Configuration::getValue("ASP_Functions");
    if(is_fathers)
    {
        function_cmd.append(functions_folder);
        function_cmd.append("fatherR1.lp ");
        function_cmd.append(functions_folder);
        function_cmd.append("fatherR2.lp ");
        function_cmd.append(functions_folder);
        function_cmd.append("fatherR3.lp ");
    }
    else
    {
        function_cmd.append(functions_folder);
        function_cmd.append("sonR1.lp ");
        function_cmd.append(functions_folder);
        function_cmd.append("sonR2.lp ");
        function_cmd.append(functions_folder);
        function_cmd.append("sonR3.lp ");
    }
    function_cmd.append(functions_folder);
    function_cmd.append("HasseD2.lp ");
    //TODO pur files in conf and handle powerset
    function_cmd.append(functions_folder);
    function_cmd.append("facts/powerSet");
    int el = function->getNumberOfRegulators();
    if(el < 10)
    {
        function_cmd.append("0");
    }
    function_cmd.append(std::to_string(el));
    function_cmd.append(".lp ");
    
    std::string clauses_file = "clause_aux";
    clauses_file.append(std::to_string((int)time(NULL)));
    clauses_file.append(".lp");

    function_cmd.append(clauses_file);
    
    std::ofstream file(clauses_file);
    file << constructFunctionClause(function);
    file.close();

    std::string result_cmd = exec(function_cmd.c_str());

    //process result to function structure
    //std::cout << result_cmd << std::endl;
    std::string rm_cmd = "rm ";
    rm_cmd.append(clauses_file);
    exec(rm_cmd.c_str());

    return parseFunctionFamily(result_cmd, function);
}


std::string ASPHelper::constructFunctionClause(Function* function){
    std::map<std::string,int> regMap = function->getRegulatorsMap();
    std::string result = "";
    for(int i = 1; i <= function->nClauses_; i++)
    {
        std::vector<std::string> clause = function->clauses_.find(i)->second;
        for(auto it = clause.begin(), end=clause.end(); it!=end;it++)
        {
           result.append("clause(");
           result.append(std::to_string(i));
           result.append(",");
           result.append(std::to_string(regMap.find((*it))->second));
           result.append("). ");
        }
    }

    return result;
   
}


std::vector<Function*> ASPHelper::parseFunctionFamily(std::string input, Function* original)
{
    //std::cout << "DEBUG original function: " << original->printFunction() << std::endl;
    std::vector<Function*> result;
    std::map<std::string,int> regMap = original->getRegulatorsMap();

    //std::cout << "DEBUG Map\n";
    //for(auto ii = regMap.begin(), ee = regMap.end(); ii != ee; ii++)
    //{
    //    std::cout << "\t" << ii->first << " => " << ii->second << "\n";
    //}

    bool process = false;
    
    std::istringstream iss(input);
    std::string line;
    while(std::getline(iss,line))
    {
        if(line.find("UNSATISFIABLE\n") != std::string::npos)
        {
            std::vector<Function*> result_;
            return result_;
        }
        
        if(line.find("Answer: ") != std::string::npos)
        {

            process = true;

        }
        else
        {
            if(process)
            {
                process = false;

                //process new function
                Function* newFunction = new Function(original->node_, 1);
                //newFunction->regulatorsMap_ = regMap;
                std::vector<std::string> splitLine = Util_h::split(line, ' ');
                int clauseId = 1;
                for(auto it = splitLine.begin(), end = splitLine.end(); it != end; it++)
                {
                    if((*it).find("selected") != std::string::npos)
                    {
                        std::vector<std::string> aux = Util_h::split((*it), '(');
                        aux = Util_h::split(aux[1], ')');
                        int value;
                        try
                        {
                            value = std::stoi(aux[0]);
                        }
                        catch(...)
                        {
                            std::cout << "WARN! Invalid value: " << aux[0] << std::endl;
                            continue;
                        }

                        //it is assumed that the power set is constructed as bit vectors of presence
                        // set 1 only has the first element
                        // set 2 only has the second element
                        // set 3 has first and second element (bits on in number 3 in binary)
                        std::bitset<16> bits (value);
                        int regulators = original->getNumberOfRegulators();
                        for(int i = 0; i < regulators; i++)
                        {
                            if(bits.test(i))
                            {
                                for(auto it2 = regMap.begin(), end2 = regMap.end(); it2!=end2; it2++)
                                {
                                    if(it2->second == i+1)
                                    {
                                        newFunction->addElementClause(clauseId, it2->first);
                                        continue;
                                    }
                                }
                            }
                        }
                        
                        clauseId++;
                    }
                }

                newFunction->level_ = original->level_ + 1;
                result.push_back(newFunction);
                //std::cout << "DEBUG read function: " << newFunction->printFunction() << std::endl;
                
            }
        }
    }

    return result;
}