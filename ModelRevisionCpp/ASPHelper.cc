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

#include <clingo.hh>

enum update_type { ASYNC = 0, SYNC, MASYNC};

std::vector<InconsistencySolution*> ASPHelper::checkConsistency(Network * network, int & optimization, int update) {

    std::vector<InconsistencySolution*> result;

    try
    {
        Clingo::Logger logger = [](Clingo::WarningCode, char const *message) {
            if(Configuration::isActive("debug"))
            {
                std::cerr << message << std::endl;
            }
        };

        Clingo::Control * ctl = new Clingo::Control({"--opt-mode=optN"}, logger, 20);

        ctl->load(Configuration::getValue("ASP_CC_BASE").c_str());

        if(network->has_ss_obs && network->has_ts_obs)
        {
            //ctl->load(Configuration::getValue("ASP_CC_SS_D").c_str());
            ctl->load(Configuration::getValue("ASP_CC_SS").c_str());
            ctl->load(Configuration::getValue("ASP_CC_D").c_str());
        }
        else
        {
            if(network->has_ss_obs)
            {
                ctl->load(Configuration::getValue("ASP_CC_SS").c_str());
            }
            else
            {
                ctl->load(Configuration::getValue("ASP_CC_D").c_str());
            }
        }
        if(network->has_ts_obs)
        {
            switch(update)
            {
                case ASYNC:
                    ctl->load(Configuration::getValue("ASP_CC_D_A").c_str());
                    break;
                case SYNC:
                    ctl->load(Configuration::getValue("ASP_CC_D_S").c_str());
                    break;
                case MASYNC:
                    ctl->load(Configuration::getValue("ASP_CC_D_MA").c_str());
                    break;
            }
        }

        ctl->load(network->input_file_network_.c_str());
        for(auto it = network->observation_files.begin(), end = network->observation_files.end(); it != end; it++)
        {
            ctl->load((*it).c_str());
        }

        ctl->ground({{"base", {}}});
        Clingo::SolveHandle sh = ctl->solve();
        if(sh.get().is_satisfiable())
        {
            for(auto &m : sh)
            {
                if(m && m.optimality_proven())
                {
                    //optimum model found
                    result.push_back(parseCCModel(m, optimization));
                }
            }
        }
        else
        {
            optimization = -1;
        }

    }
    catch (std::exception const &e) {
        std::cerr << "failed to check consistency: " << e.what() << std::endl;
    }

    return result;
}

InconsistencySolution * ASPHelper::parseCCModel(const Clingo::Model &m, int & optimization)
{
    InconsistencySolution* inconsistency = new InconsistencySolution();
    int count = 0;
    for(auto &atom : m.symbols())
    {
        std::string name(atom.name());
        Clingo::SymbolSpan args = atom.arguments();

        if(name.compare("vlabel") == 0)
        {
            if(args.size() > 3)
            {
                inconsistency->addVLabel(args[0].to_string(), args[2].to_string(), args[3].number(), args[1].number());
            }
            else
            {
                inconsistency->addVLabel(args[0].to_string(), args[1].to_string(), args[2].number(), 0);
            }
            continue;
        }
        if(name.compare("r_gen") == 0)
        {
            inconsistency->addGeneralization(args[0].to_string());
            continue;
        }
        if(name.compare("r_part") == 0)
        {
            inconsistency->addParticularization(args[0].to_string());
        }
        if(name.compare("repair") == 0)
        {
            count++;
            continue;
        }
        if(name.compare("update") == 0)
        {
            inconsistency->addUpdate(args[1].number(), args[0].to_string(), args[2].to_string());
            continue;
        }
        if(name.compare("topologicalerror") == 0)
        {
            inconsistency->addTopologicalError(args[0].to_string());
            continue;
        }

    }
    optimization = count;
    return inconsistency;
}

/*
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

}
*/

int ASPHelper::parseNetwork(Network * network) {

    int result = 1;

    std::ifstream file;
    file.open(network->input_file_network_);
    if(!file.good())
    {
        std::fprintf(stderr, "ERROR!\tCannot open file '%s'.\n", network->input_file_network_.c_str());
        file.close();
        return -2;
    }
    int countLine = 0;

    std::string line;
    while(std::getline(file,line))
    {
        countLine++;
        line.erase(std::remove_if(line.begin(),line.end(),isspace),line.end());
        if(line.find(").") != std::string::npos)
        {
            std::vector<std::string> predicates = Util_h::split(line,')');
            for (int i = 0; i < (int)predicates.size()-1; i++)
            {
                //fix to allow multiple predicates per line
                predicates[i] += ").";
                if(i>0)
                {
                    predicates[i].erase(0,1);
                }

                std::vector<std::string> split = Util_h::split(predicates[i],'(');
                if(split[0].compare("edge") == 0)
                {
                    split = Util_h::split(split[1], ')');
                    split = Util_h::split(split[0], ',');
                    if(split.size() != 3)
                    {
                        std::cout << "WARN!\tEdge not recognised in line " << countLine << ": " << predicates[i] << std::endl;
                        result = -1;
                        continue;
                    }
                    if((!islower(split[0][0]) && !isdigit(split[0][0])) ||  (!islower(split[1][0]) && !isdigit(split[1][0])))
                    {
                        std::cout << "WARN!\tInvalid node argument in line " << countLine << ": " << predicates[i] << std::endl;
                        return -2;
                    }
                    std::string startId = split[0];
                    std::string endId = split[1];
                    int sign;
                    try {
                        sign = std::stoi(split[2]);
                    }
                    catch(...)
                    {
                        std::cout << "WARN!\tInvalid edge sign: " << split[2] << " on line " << countLine << " on edge " << predicates[i] << std::endl;
                        return -2;
                    }
                    if(sign != 0 && sign != 1 )
                    {
                        std::cout << "WARN!\tInvalid edge sign on line " << countLine << " on edge " << predicates[i] << std::endl;
                        return -2;
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
                    if(split.size() != 2)
                    {
                        continue;
                    }
                    if((!islower(split[0][0]) && !isdigit(split[0][0])) ||  (!islower(split[1][0]) && !isdigit(split[1][0])))
                    {
                        std::cout << "WARN!\tInvalid node argument in line " << countLine << ": " << predicates[i] << std::endl;
                        return -2;
                    }
                    std::string startId = split[0];
                    std::string endId = split[1];

                    Edge* e = network->getEdge(startId, endId);
                    if(e != nullptr)
                    {
                        e->setFixed();
                    }
                    else
                    {
                        std::cout << "WARN!\tUnrecognised edge on line " << countLine << ": " << predicates[i] << " Ignoring..." << std::endl;
                    }
                    continue;
                }
                if(split[0].compare("functionOr") == 0)
                {
                    split = Util_h::split(split[1], ')');
                    split = Util_h::split(split[0], ',');
                    if(split.size() != 2)
                    {
                        std::cout << "WARN!\tfunctionOr not recognised on line " << countLine << ": " << predicates[i] << std::endl;
                        result = -1;
                        continue;
                    }
                    if((!islower(split[0][0]) && !isdigit(split[0][0])))
                    {
                        std::cout << "WARN!\tInvalid node argument in line " << countLine << ": " << predicates[i] << std::endl;
                        return -2;
                    }
                    network->addNode(split[0]);
                    
                    if(split[1].find("..") != std::string::npos)
                    {
                        split = Util_h::split(split[1], '.');
                        int range;
                        try {
                            range = std::stoi(split[split.size()-1]);
                        }
                        catch(...)
                        {
                            std::cout << "WARN!\tInvalid range limit: " << split[split.size()-1] << " on line " << countLine << " in " << predicates[i] << " It must be an integer greater than 0." << std::endl;
                            return -2;
                        }
                        if(range < 1)
                        {
                            std::cout << "WARN!\tInvalid range limit: " << range << " on line " << countLine << " in " << predicates[i] << " It must be an integer greater than 0." << std::endl;
                            return -2;
                        }

                    }
                    else
                    {
                        int range;
                        try {
                            range = std::stoi(split[1]);
                            if(range < 1)
                            {
                                std::cout << "WARN!\tInvalid range limit: " << range << " on line " << countLine << " in " << predicates[i] << " It must be an integer greater than 0." << std::endl;
                                return -2;
                            }
                        }
                        catch(...)
                        {
                            std::cout << "WARN!\tInvalid functionOr range definition in line " << countLine << ": " << predicates[i] << std::endl;
                            return -2;
                        }
                        
                    }

                    //Node* node = network->addNode(split[0]);
                    //if(split[1].find("..") != std::string::npos)
                    //{
                    //    std::vector<std::string> aux_split = Util_h::split(split[1], '.');
                    //    split[1] = aux_split[aux_split.size()-1];
                    //}

                    //Function* f = new Function(split[0]);
                    //node->addFunction(f);
                    continue;
                }
                if(split[0].compare("functionAnd") == 0)
                {
                    split = Util_h::split(split[1], ')');
                    split = Util_h::split(split[0], ',');
                    if(split.size() != 3)
                    {
                        std::cout << "WARN!\tFunctionAnd not recognised on line " << countLine << ": " << predicates[i] << std::endl;
                        result = -1;
                        continue;
                    }

                    if((!islower(split[0][0]) && !isdigit(split[0][0])) ||  (!islower(split[2][0]) && !isdigit(split[2][0])))
                    {
                        std::cout << "WARN!\tInvalid node argument in line " << countLine << ": " << predicates[i] << std::endl;
                        return -2;
                    }

                    Node * node = network->getNode(split[0]);
                    if(node == nullptr)
                    {
                        std::cout << "WARN!\tNode not recognised or not yet defined: " << split[0] << " on line " << countLine << " in " << predicates[i] << std::endl;
                        result = -1;
                        continue;
                    }
                    Node * node2 = network->getNode(split[2]);
                    if(node2 == nullptr)
                    {
                        std::cout << "WARN!\tNode not recognised or not yet defined: " << split[2] << " on line " << countLine << " in " << predicates[i] << std::endl;
                        result = -1;
                        continue;
                    }

                    int clauseId;
                    try {
                        clauseId = std::stoi(split[1]);
                    }
                    catch(...)
                    {
                        std::cout << "WARN!\tInvalid clause Id: " << split[1] << " on line " << countLine << " in " << predicates[i] << std::endl;
                        result = -1;
                        continue;
                    }

                    if(clauseId < 1)
                    {
                        std::cout << "WARN!\tInvalid clause Id: " << split[1] << " on line " << countLine << " in " << predicates[i] << std::endl;
                        result = -1;
                        continue;
                    }

                    node->getFunction()->addElementClause(clauseId, split[2]);
                    continue;
                }

            }

        }
    }

    file.close();
    return result;

}

/*
std::vector<InconsistencySolution*> ASPHelper::parseFunctionRepairResults(std::vector<std::vector<std::string>> results) {
    std::vector<InconsistencySolution*> result;   
    for(auto it = results.begin(), end= results.end(); it!=end; it++)
    {
        InconsistencySolution* inconsistency = new InconsistencySolution();
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
                    value = std::stoi(split[2]);
                }
                catch(...)
                {
                    std::cout << "WARN! Invalid value in label: " << split[2] << std::endl;
                    continue;
                }
                inconsistency->addVLabel(split[0], split[1], value);
                continue;
            }
            
            if(split[0].compare("r_gen") == 0)
            {
                split = Util_h::split(split[1], ')');
                inconsistency->addGeneralization(split[0]);
                continue;
            }

            if(split[0].compare("r_part") == 0)
            {
                split = Util_h::split(split[1], ')');
                inconsistency->addParticularization(split[0]);
                continue;
            }

        }

        result.push_back(inconsistency);
    }

    return result;
}
*/

/*
std::vector<Function*> ASPHelper::getFunctionReplace(Function* function, bool is_fathers, std::string filename) {
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
    
    std::string clauses_file = "clause_aux.";
    clauses_file.append(Util_h::getFilename(filename));
    //this is to avoid colisions if necessary
    //clauses_file.append(".");
    //clauses_file.append(std::to_string((int)time(NULL)));
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
*/

/*
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
*/

/*
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
                Function* newFunction = new Function(original->node_);
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
*/