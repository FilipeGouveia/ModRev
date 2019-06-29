#include "main.h"
#include "Configuration.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ASPHelper.h"
#include "Network.h"
#include <time.h>

Network * network = new Network();

int main(int argc, char ** argv) {


    Configuration::parseConfig();


    std::string input_file_network, output_file, log_file;
    int f_prob, e_prob, r_prob, a_prob;

    //arguments usage: prog.exe <input network> <f_prob> <e_prob> <r_prob> <a_prob> <opt:output_file> <opt:log_file>

    if(argc < 6)
    {
        std::cout << "Invalid number of arguments: " << argc << std::endl;
        return -1;
    }


    process_arguments(argc, argv, input_file_network, output_file, log_file, f_prob, e_prob, r_prob, a_prob);


    unsigned int seed = time(NULL);
    srand(seed);


    std::ofstream log_s;
    //output_s.open(output_file);
    log_s.open(log_file);

    log_s << "Seed: " << seed << std::endl;

    ASPHelper::parseNetwork(input_file_network, network);

    //main function that messes up the model
    modelMessing(log_s, f_prob, e_prob, r_prob, a_prob);

    printNetwork(output_file);

    log_s.close();
 
}


// Function that initializes the program.
// Can process optional arguments or configurations
void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file, std::string & log_file, int & f_prob, int & e_prob, int & r_prob, int & a_prob) {

    input_file_network = argv[1];

    try {
        f_prob = std::stoi(argv[2]);
    }
    catch(...)
    {
        std::cout << "WARN! Invalid f_prob: " << argv[2] << std::endl;
        f_prob = 0;
    }
    try {
        e_prob = std::stoi(argv[3]);
    }
    catch(...)
    {
        std::cout << "WARN! Invalid e_prob: " << argv[3] << std::endl;
        e_prob = 0;
    }
    try {
        r_prob = std::stoi(argv[4]);
    }
    catch(...)
    {
        std::cout << "WARN! Invalid r_prob: " << argv[4] << std::endl;
        r_prob = 0;
    }
    try {
        a_prob = std::stoi(argv[5]);
    }
    catch(...)
    {
        std::cout << "WARN! Invalid a_prob: " << argv[5] << std::endl;
        a_prob = 0;
    }
    if(argc > 6)
    {
        output_file = argv[6];
    }
    else
    {
        output_file = input_file_network;
        output_file.append(".");
        output_file.append(std::to_string(f_prob));
        output_file.append("_");
        output_file.append(std::to_string(e_prob));
        output_file.append("_");
        output_file.append(std::to_string(r_prob));
        output_file.append("_");
        output_file.append(std::to_string(a_prob));
        output_file.append(".output");
    }

    if(argc > 7)
    {
        log_file = argv[7];
    }
    else
    {
        log_file = output_file;
        log_file.append(".log");
    }

    return;
}

bool getProbability(int prob)
{
    return (rand() % 100) < prob;
}


void modelMessing(std::ofstream & log_s, int f_prob, int e_prob, int r_prob, int a_prob) {


    //first add and remove edges and flip the sign of the pre-existing edges
    std::vector<Node *> nodes = network->getNodes();
    for(auto node = nodes.begin(), endNode = nodes.end(); node != endNode; node++ )
    {
        Node * originalN = (*node);
        if(originalN->fixed_)
        {
            continue;
        }
        Function * originalF = originalN->regFunction_;
        std::map<std::string,int> originalMap = originalF->getRegulatorsMap();
        int numberRegulators = (int)originalMap.size();
        std::vector<Edge*> listEdgesFinal;

        
        for(auto it = originalMap.begin(), end = originalMap.end(); it!= end; it++)
        {
            Edge* e = network->getEdge(it->first, originalF->node_);
            if(e!=nullptr && !e->isFixed())
            {
                if(getProbability(r_prob))
                {
                    originalF->domainChanged = true;
                    log_s << "removed edge(" << e->start_->id_ << ", " << e->end_->id_ << ")." << std::endl;
                    network->removeEdge(e);
                    //e->deleted_ = true;
                    numberRegulators--;
                }
                // if we do not remove the edge, lets try to flip the sign
                else
                {
                    listEdgesFinal.push_back(e);
                    if(getProbability(e_prob))
                    {
                        log_s << "flip the sign of edge(" << e->start_->id_ << ", " << e->end_->id_ << ")." << std::endl;
                        e->flipSign();
                    }
                }
            }
            if(e!=nullptr && e->isFixed())
            {
                listEdgesFinal.push_back(e);
            }
        }

        
        for(auto it = nodes.begin(), end = nodes.end(); it!= end; it++)
        {
            //no more than 12 regulators are allowed
            if(numberRegulators >= 12)
            {
                break;
            }

            bool isOriginalRegulator = false;
            for(auto it2 = originalMap.begin(), end2 = originalMap.end(); it2!=end2; it2++)
            {
                if((*it)->id_.compare(it2->first) == 0)
                {
                    isOriginalRegulator = true;
                    break;
                }
            }
            if(!isOriginalRegulator)
            {
                if(getProbability(a_prob))
                {
                    int sign = 0;
                    if(getProbability(50))
                        sign = 1;
                    Edge* newEdge = new Edge((*it), originalN, sign);
                    originalF->domainChanged = true;
                    log_s << "added edge(" << newEdge->start_->id_ << ", " << newEdge->end_->id_ << ", " << sign << ")." << std::endl;
                    network->addEdge(newEdge);
                    numberRegulators++;
                    listEdgesFinal.push_back(newEdge);
                }
            }

        }

        
        //now lets deal with the functions

        //have to check if there are regulators left
        if(numberRegulators < 1)
        {
            log_s << "removed function from " << originalN->id_ << " (turned node input)." << std::endl;
            originalN->regFunction_ = nullptr;
            continue;
        }

        
        if(originalF->domainChanged || getProbability(f_prob))
        {
            bool isGeneralize = getProbability(50);
            Function * candidate = originalF;
            std::vector<Function*> tCandidates;
            if(originalF->domainChanged)
            {
                candidate = new Function(originalN->id_, 1);
                for(auto it = listEdgesFinal.begin(), end = listEdgesFinal.end(); it!=end; it++)
                {
                    candidate->addElementClause(1, (*it)->start_->id_);
                }
                isGeneralize = true;
                tCandidates.push_back(candidate);
            }
            else
            {
                if(numberRegulators == 1 || numberRegulators > 12)
                {
                    continue;
                }
                tCandidates = ASPHelper::getFunctionReplace(candidate, isGeneralize);
                if(tCandidates.empty())
                {
                    isGeneralize = !isGeneralize;
                    tCandidates = ASPHelper::getFunctionReplace(candidate, isGeneralize);
                    if(tCandidates.empty())
                    {
                        //no functions are possible to change
                        continue;
                    }
                }
            }

            bool isNonComparable = Configuration::isActive("nonComparableFunctions");
            while(!tCandidates.empty())
            {
                candidate = tCandidates.front();
                tCandidates.erase(tCandidates.begin());
                
                if(getProbability(50) || numberRegulators == 1 || numberRegulators > 12)
                {
                    break;
                }
                if(isNonComparable && getProbability(50))
                {
                    isGeneralize = !isGeneralize;
                }
                std::vector<Function*> tauxCandidates = ASPHelper::getFunctionReplace(candidate, isGeneralize);
                if(!tauxCandidates.empty())
                    tCandidates.insert(tCandidates.end(),tauxCandidates.begin(),tauxCandidates.end());
                
            }

            originalN->regFunction_ = candidate;
            log_s << "changed function of " << originalN->id_ << " to " << candidate->printFunction() << std::endl;

        }

        
    }

}

void printNetwork(std::string output_file)
{
    std::ofstream output_s;
    output_s.open(output_file);
    if(!output_s.is_open() || !output_s.good())
    {
        std::cout << "OUTPUT FILE not opennor bad" << std::endl;
        return;
    }

    //print edges
    for(auto it = network->edges_.begin(), end = network->edges_.end(); it!= end; it++)
    {
        output_s << "edge(" << (*it)->start_->id_ << ", " << (*it)->end_->id_ << ", " << (*it)->sign_ << ")." << std::endl;
    }

    //print functions
    std::vector<Node*> nodes = network->getNodes();
    for(auto it = nodes.begin(), end = nodes.end(); it!=end; it++)
    {
        Function * f = (*it)->getFunction();
        if(f != nullptr)
        {
            output_s << "functionOr(" << (*it)->id_ << ", 1";
            if(f->nClauses_ > 1)
            {
                output_s << ".." << f->nClauses_;
            }
            output_s << ")." << std::endl;

            //clauses
            for(auto it2 = f->clauses_.begin(), end2 = f->clauses_.end(); it2 != end2; it2++)
            {
                for(auto it3 = it2->second.begin(), end3 = it2->second.end(); it3!=end3; it3++)
                {
                    output_s << "functionAnd(" << (*it)->id_ << ", " << it2->first << ", " << (*it3) << ")." << std::endl;
                }
            }

        }
    }

    output_s.close();
    return;
}