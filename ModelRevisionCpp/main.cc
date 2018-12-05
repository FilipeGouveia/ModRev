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
    
}


// Function that initializes the program.
// Can process optional arguments or configurations
void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file) {

    input_file_network = argv[1];

    if(argc > 2)
        output_file = argv[2];

    return;
}



//Model revision procedure
// 1) tries to repair functions
// 2) tries to flip the sign of the edges
// 3) tries to add or remove edges
void modelRevision(std::string input_file_network) {

    int optimization = -2;

    std::vector<InconsistencySolution*> fInconsistencies = checkConsistencyFunc(input_file_network, optimization);
    if(optimization == 0)
    {
        return;
    }

    //At this point we have an inconsistent network with node candidates to be repaired

    //for each possible inconsistency solution/labelling, try to make the model consistent
    InconsistencySolution * bestSolution = nullptr;
    for(auto it = fInconsistencies.begin(), end = fInconsistencies.end(); it != end; it++)
    {
        repairInconsistencies((*it));

        //TODO
        if(!(*it)->hasImpossibility)
        {
            if(bestSolution == nullptr || (*it)->getNTopologyChanges() < bestSolution->getNTopologyChanges())
            {
                bestSolution = (*it);
                if(Configuration::isActive("debug"))
                    std::cout << "DEBUG: found solution with " << bestSolution->getNTopologyChanges() << " topology changes.\n";
                if(bestSolution->getNTopologyChanges() == 0 && !Configuration::isActive("allOpt"))
                    break;
            }
        }  
    }

    if(bestSolution == nullptr)
    {
        std::cout << "### It was not possible to repair the model." << std::endl;
        return;
    }
    
    //TODO
    if(Configuration::isActive("allOpt"))
    {
        //TODO: remove duplicates
        for(auto it = fInconsistencies.begin(), end = fInconsistencies.end(); it != end; it++)
        {
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: checking for printing solution with " << (*it)->getNTopologyChanges() << " topology changes\n";
            if(!(*it)->hasImpossibility && (*it)->getNTopologyChanges() == bestSolution->getNTopologyChanges())
            {
                (*it)->printSolution();
            }
        }

    }
    else
    {
        bestSolution->printSolution();
    }

}


//function reponsible to check the consistency of a model and return a set of possible function inconsistencies
std::vector<InconsistencySolution*> checkConsistencyFunc(std::string input_file_network, int & optimization) {

    std::vector<std::vector<std::string>> result_raw;
    std::vector<InconsistencySolution*> result;
    
    //consistency check
    if(Configuration::isActive("check_ASP"))
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

    if(optimization > 0 && Configuration::isActive("check_ASP"))
    {
        //parse the raw results to an internal representation.
        //this should be done at ASP level in the check consistency function
        //TODO
        result = ASPHelper::parseFunctionRepairResults(result_raw);
    }

    return result;
}



//This function receives an inconsistent model with a set of nodes to be repaired and try to repair the target nodes making the model consistent
//returns the set of repair operations to be applied
void repairInconsistencies(InconsistencySolution* inconsistency)
{

    //repair each inconsistent node
    for(auto it = inconsistency->iNodes_.begin(), end = inconsistency->iNodes_.end(); it != end; it++)
    {
        repairNodeConsistency(inconsistency, it->second);
        if(inconsistency->hasImpossibility)
        {
            //one of the nodes was not possible to repair
            return;
        }
    }

}

//This function repair a given node and determines all possible solutions
void repairNodeConsistency(InconsistencySolution* inconsistency, InconsistentNode* iNode)
{
    std::vector<std::vector<Function*>> candidates;

    if(Configuration::isActive("function_ASP"))
    {

        //each function must have a list of replacement candidates and each must be tested until it works
        Function* originalF = network->getNode(iNode->id_)->getFunction();
        if(originalF == nullptr)
        {
            std::cout << "WARN: Inconsistent node " << iNode->id_ << " without regulatory function." << std::endl;
            inconsistency->hasImpossibility = true;
            return;
        }

        //if the function only has 1 regulator then it is not possible to change the function
        //better try to flip the sign of the edge
        //check top function for the necessity of flipping an edge
        if(originalF->getNumberOfRegulators() < 2 || !checkPointFunction(inconsistency, originalF, iNode->generalization_))
        {
            repairNodeConsistencyWithTopologyChanges(inconsistency, iNode);
            return;
        }

        // get the possible candidates to replace the inconsistent function
        bool functionRepaired = false;
        int repairedFunctionLevel = -1;
        std::vector<Function*> tCandidates = ASPHelper::getFunctionReplace(originalF,iNode->generalization_);
        while(!tCandidates.empty())
        {
            Function* candidate = tCandidates.front();
            tCandidates.erase (tCandidates.begin());
            
            if(functionRepaired && candidate->level_ > repairedFunctionLevel)
            {
                //function is from a higher level than expected
                continue;
            }

            if(isFuncConsistentWithLabel(inconsistency, candidate))
            {

                RepairSet * repairSet = new RepairSet();
                repairSet->addRepairedFunction(candidate);
                inconsistency->addRepairSet(iNode->id_, repairSet);
                functionRepaired = true;
                repairedFunctionLevel = candidate->level_;
                if(!Configuration::isActive("showAllFunctions"))
                {
                    break;
                }
            }

            std::vector<Function*> tauxCandidates = ASPHelper::getFunctionReplace(candidate,iNode->generalization_);
            if(!tauxCandidates.empty())
                tCandidates.insert(tCandidates.end(),tauxCandidates.begin(),tauxCandidates.end());
            
        }
        if(!functionRepaired)
        {
            //not possible to repair without topology changes
            //For 1 profile is not suposed to reach this point
            repairNodeConsistencyWithTopologyChanges(inconsistency, iNode);
        }
        
    }
    else
    {
    //TODO support other solvers
    } 

    return;
}

bool isFuncConsistentWithLabel(InconsistencySolution* labeling, Function* f)
{
    //verify for each profile
    for(auto it = labeling->vlabel_.begin(), end = labeling->vlabel_.end(); it != end; it++)
    {
        if(!isFuncConsistentWithLabel(labeling, f, it->first))
            return false;
    }
    return true;
}

bool isFuncConsistentWithLabel(InconsistencySolution* labeling, Function* f, std::string profile)
{
    for(int i = 1; i <= f->nClauses_; i++)
    {
        bool isClauseSatisfiable = true;
        std::vector<std::string> clause = f->clauses_[i];
        for(auto it = clause.begin(), end = clause.end(); it!=end; it++)
        {
            Edge* e = network->getEdge((*it), f->node_);
            if(e != nullptr)
            {
                //positive interaction
                if(e->getSign() > 0)
                {
                    if(labeling->vlabel_[profile][(*it)] == 0)
                    {
                        isClauseSatisfiable = false;
                        break;
                    }
                }
                //negative interaction
                else
                {
                    if(labeling->vlabel_[profile][(*it)] > 0)
                    {
                        isClauseSatisfiable = false;
                        break;
                    }
                }
            }
            else{
                std::cout << "WARN: Missing edge from " << (*it) << " to " << f->node_ << std::endl;
                return false;
            }
        }
        if(isClauseSatisfiable && labeling->vlabel_[profile][f->node_] == 1)
            return true;

    }
    return labeling->vlabel_[profile][f->node_] == 0;
}

bool checkPointFunction(InconsistencySolution* labeling, Function* f, bool generalize)
{
    //for each profile
    for(auto it = labeling->vlabel_.begin(), end = labeling->vlabel_.end(); it != end; it++)
    {
        if(!checkPointFunction(labeling, f, it->first, generalize))
            return false;
    }
    return true;
}
//checks thhe top or bottom function for consistency.
// Allows to check if it is possible to repair a function without changing the topology
bool checkPointFunction(InconsistencySolution* labeling, Function* f, std::string profile, bool generalize){
    std::map<std::string,int> map = f->getRegulatorsMap();
    if(generalize)
    {
        // disjunction of all regulators
        for(auto it = map.begin(), end = map.end(); it!= end; it++)
        {
            Edge* e = network->getEdge(it->first, f->node_);
            if(e != nullptr)
            {
                //positive interaction
                if(e->getSign() > 0 && labeling->vlabel_[profile][it->first] > 0)
                {
                    return true;
                }
                //negative interaction
                if(e->getSign() == 0 && labeling->vlabel_[profile][it->first] == 0)
                {
                    return true;
                }
            }
            else{
                std::cout << "WARN: Missing edge from " << it->first << " to " << f->node_ << std::endl;
                return false;
            }
        }
        return false;
    }
    else{
        // conjunction of all regulators
        // as this is a particularization, it is needed more zeros and the function evaluation should be 0
        for(auto it = map.begin(), end = map.end(); it!= end; it++)
        {
            Edge* e = network->getEdge(it->first, f->node_);
            if(e != nullptr)
            {
                //positive interaction
                if(e->getSign() > 0 && labeling->vlabel_[profile][it->first] == 0)
                {
                    return true;
                }
                //negative interaction
                if(e->getSign() == 0 && labeling->vlabel_[profile][it->first] > 0)
                {
                    return true;
                }
            }
            else{
                std::cout << "WARN: Missing edge from " << it->first << " to " << f->node_ << std::endl;
                return false;
            }
        }
        return false;
    }
    return true;

}

//this only works for flipping edges
void repairNodeConsistencyWithTopologyChanges(InconsistencySolution* solution, InconsistentNode* iNode)
{
    Function * f = network->getNode(iNode->id_)->regFunction_;
    std::map<std::string,int> map = f->getRegulatorsMap();
    std::vector<Function*> tCandidates;
    tCandidates.push_back(f);
    int bestFunctionLevel = -1;
    //this is used to check if we can repair the node without changing the function.
    //If so, do not add a new function to the solution, only include flipping the edge

    while(!tCandidates.empty())
    {
        Function* candidate = tCandidates.front();
        tCandidates.erase (tCandidates.begin());

        if(bestFunctionLevel >= 0 && candidate->level_ > bestFunctionLevel)
        {
            //function is from a higher level than expected
            continue;
        }

        //try to flip each edge
        //TODO change this to contemplate all solutions with all the possible edges flip from 1 to n regulators
        for(auto it = map.begin(), end = map.end(); it!= end; it++)
        {
             Edge* e = network->getEdge(it->first, f->node_);
             if(e!=nullptr)
             {
                 if(!e->isFixed())
                 {
                    e->flipSign();
                    if(isFuncConsistentWithLabel(solution, candidate))
                    {
                        e->flipSign();
                        if(Configuration::isActive("debug"))
                            std::cout << "REPAIR: Try to change the sign of the edge from " << it->first << " to " << f->node_ << std::endl;
                        
                        RepairSet * repairSet = new RepairSet();
                        repairSet->addFlippedEdge(e);
                        if(candidate->level_ > 0)
                        {
                            //Only add a function repair operations if the candidate is not the original
                            repairSet->addRepairedFunction(candidate);
                        }
                        solution->addRepairSet(iNode->id_, repairSet);
                        if(!Configuration::isActive("showAllFunctions"))
                        {
                            return;
                        }
                    }
                 }
             }
             else{
                std::cout << "WARN: Missing edge from " << it->first << " to " << f->node_ << std::endl;
                continue;
            }
        }

        //renew candidates
        std::vector<Function*> tauxCandidates = ASPHelper::getFunctionReplace(candidate, iNode->generalization_);
        if(!tauxCandidates.empty())
            tCandidates.insert(tCandidates.end(),tauxCandidates.begin(),tauxCandidates.end());

    }

    //If the end of this method is reached means that no solution was found
    solution->hasImpossibility = true;
    std::cout << "WARN: Not possible to flip an edge to repair function " << f->node_ << std::endl;
    return;
}
