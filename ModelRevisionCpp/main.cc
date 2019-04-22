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

/*     std::vector<Edge*> vt;
    Node* n = network->getNode("c3");
    std::map<std::string,int> map = n->getFunction()->getRegulatorsMap();
    for(auto it = map.begin(), end = map.end(); it!= end; it++)
    {
            Edge* e = network->getEdge(it->first, "c3");
            vt.push_back(e);
    }
    for(int i = 1; i < 4; i++)
    {
        auto aux = getEdgesCombinations(vt,i);
        std::cout << "#### edges with " << i << " combinations\n";
        for(auto it1 = aux.begin(), end1 = aux.end(); it1!=end1; it1++)
        {
            std::cout << "\t---\n";
            for(auto it2 = (*it1).begin(), end2 = (*it1).end(); it2!=end2; it2++)
            {
                std::cout << "\t from " << (*it2)->start_->id_ << "\n";
            }
        }
    } */
    
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
        //FG
        if(it->second->repairType > 2)
        {
            std::cout << "#FOUND a node with double inconsistency\n";
            //inconsistency->hasImpossibility = true;
            testDouble(inconsistency, it->second);
            //throw std::invalid_argument( "end of double inconsistency" );
            //return;
        }
        else
        {
        repairNodeConsistency(inconsistency, it->second);
        if(inconsistency->hasImpossibility)
        {
            //one of the nodes was not possible to repair
            return;
        }
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
        //check top function for the necessity of flipping an edge <- yhis only works for single profile
        
        //if(originalF->getNumberOfRegulators() < 2 || !checkPointFunction(inconsistency, originalF, iNode->generalization_))
        if(originalF->getNumberOfRegulators() < 2)
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
        if(isClauseSatisfiable)
            return labeling->vlabel_[profile][f->node_] == 1;

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
//TODO consider this function for no flipping edges
void repairNodeConsistencyWithTopologyChanges(InconsistencySolution* solution, InconsistentNode* iNode)
{

    std::vector<Edge*> emptyList;
    bool solFound = repairNodeConsistencyFlippingEdges(solution, iNode, emptyList, emptyList);

    //try to add or remove edges
    if(!solFound)
    {
        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: Not possible to only flip edges to repair function " << iNode->id_ << std::endl;

        repairNodeConsistencyByRegulators(solution, iNode);
    }

}


bool repairNodeConsistencyFlippingEdges(InconsistencySolution* solution, InconsistentNode* iNode, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges)
{
    Function * f = network->getNode(iNode->id_)->regFunction_;
    std::map<std::string,int> map = f->getRegulatorsMap();
    std::vector<Edge*> listEdges;
    for(auto it = map.begin(), end = map.end(); it!= end; it++)
    {
        Edge* e = network->getEdge(it->first, f->node_);
        if(e!=nullptr && !e->isFixed())
        {
            listEdges.push_back(e);
        }
    }

    if(Configuration::isActive("debug"))
        std::cout << "DEBUG: searching solution flipping edges for " << iNode->id_ << "\n";

    bool solFound = false;
    for(int nEdges = 1; nEdges <= (int)listEdges.size(); nEdges++)
    {
        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: testing with " << nEdges << " edge flips\n";
        std::vector<std::vector<Edge*>> eCandidates = getEdgesCombinations(listEdges, nEdges);
        std::vector<Function*> fCandidates;
        fCandidates.push_back(f);
        int bestFunctionLevel = -1;
        //This is used to check if we can repair the node without changing the function.
        //If so, do not add a new function to the solution, only include flipping the edge.

        while(!fCandidates.empty())
        {
            Function* candidate = fCandidates.front();
            fCandidates.erase (fCandidates.begin());

            if(bestFunctionLevel >= 0 && candidate->level_ > bestFunctionLevel)
            {
                //function is from a higher level than expected
                continue;
            }

            //for each set of flipping edges
            for(auto it = eCandidates.begin(), end = eCandidates.end(); it!= end; it++)
            {
                //flip all edges
                for(auto itEdge = (*it).begin(), endEdge = (*it).end(); itEdge != endEdge; itEdge++)
                {
                    Edge* e = (*itEdge);
                    e->flipSign();
                    if(Configuration::isActive("debug"))
                        std::cout << "DEBUG: flip edge from " << e->start_->id_ << "\n";
                }
                bool isSol = false;
                if(Configuration::isActive("debug"))
                        std::cout << "DEBUG: testing function " << candidate->printFunction() << "\n";
                if(isFuncConsistentWithLabel(solution, candidate))
                {
                    if(Configuration::isActive("debug"))
                        std::cout << "DEBUG: found solution with " << nEdges << " edges flipped\n";
                    isSol = true;
                    solFound = true;
                }
                //put network back to normal
                RepairSet * repairSet = new RepairSet();
                for(auto itEdge = (*it).begin(), endEdge = (*it).end(); itEdge != endEdge; itEdge++)
                {
                    Edge* e = (*itEdge);
                    e->flipSign();
                    if(Configuration::isActive("debug"))
                        std::cout << "DEBUG: return flip edge from " << e->start_->id_ << "\n";
                    if(isSol)
                    {
                        repairSet->addFlippedEdge(e);
                    }
                }
                if(isSol)
                {
                    //Only add a function repair operations if the candidate is not the original
                    if(candidate->level_ > 0)
                    {
                        if(Configuration::isActive("debug"))
                            std::cout << "DEBUG: solution without original function\n";
                        repairSet->addRepairedFunction(candidate);
                    }

                    //add and remove edges in solution repair set
                    for(auto itRem = removedEdges.begin(), endRem = removedEdges.end(); itRem != endRem; itRem++)
                    {
                        repairSet->removeEdge((*itRem));
                    }
                    for(auto itAdd = addedEdges.begin(), endAdd = addedEdges.end(); itAdd != endAdd; itAdd++)
                    {
                        repairSet->removeEdge((*itAdd));
                    }

                    solution->addRepairSet(iNode->id_, repairSet);
                    bestFunctionLevel = candidate->level_;
                    if(!Configuration::isActive("allOpt"))
                    {
                        if(Configuration::isActive("debug"))
                            std::cout << "DEBUG: no more solutions - allOpt\n";
                        return true;
                    }
                }
            }
            if(bestFunctionLevel >= 0 && !Configuration::isActive("showAllFunctions"))
            {
                if(Configuration::isActive("debug"))
                    std::cout << "DEBUG: no more function solutions\n";
                return true;           
            }

        
            if(candidate->getNumberOfRegulators() < 2)
            {
                if(Configuration::isActive("debug"))
                    std::cout << "DEBUG: function with 1 regulator\n";
                //there is no possible condidates for 1 regulator function
                break;
            }
            //renew candidates if solution level not found yet
            if(bestFunctionLevel < 0 || candidate->level_ < bestFunctionLevel)
            {
                if(Configuration::isActive("debug"))
                    std::cout << "DEBUG: updating solutions\n";
                std::vector<Function*> fauxCandidates = ASPHelper::getFunctionReplace(candidate, iNode->generalization_);
                if(!fauxCandidates.empty())
                    fCandidates.insert(fCandidates.end(),fauxCandidates.begin(),fauxCandidates.end());
            }

        }

        //If the end of this method is reached means that no solution was found with nEdges
        if(solFound)
        {
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: ready to end with " << nEdges << " edges flipped\n";
            break;
        }
        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: reached the end of " << nEdges << " without solution\n";
    }
    
    return solFound;
}


std::vector<std::vector<Edge *>> getEdgesCombinations(std::vector<Edge *> edges, int n)
{
    if(n == 0)
    {
        std::vector<std::vector<Edge *>> result;
        return result;
    }
    return getEdgesCombinations(edges, n, 0);
}

std::vector<std::vector<Edge *>> getEdgesCombinations(std::vector<Edge *> edges, int n, int indexStart)
{
    std::vector<std::vector<Edge *>> result;

    for(int i = indexStart; i <= (int)edges.size() - n; i++)
    {
        if(n > 1)
        {
            std::vector<std::vector<Edge *>> aux = getEdgesCombinations(edges, n-1, i+1);
            for(auto it = aux.begin(), end=aux.end(); it!=end; it++)
            {
                (*it).push_back(edges[i]);
                result.push_back((*it));
            }

        }
        else
        {
            std::vector<Edge*> finalAux;
            finalAux.push_back(edges[i]);
            result.push_back(finalAux);
        }
    }

    return result;
}


//TODO make this functions only one
void repairNodeConsistencyByRegulators(InconsistencySolution* solution, InconsistentNode* iNode)
{
    if(Configuration::isActive("debug"))
        std::cout << "DEBUG: searching solution adding or removing edges for " << iNode->id_ << "\n";
    Node * originalN = network->getNode(iNode->id_);
    Function * originalF = originalN->regFunction_;

    std::map<std::string,int> originalMap = originalF->getRegulatorsMap();
    std::vector<Edge*> listEdgesRemove;
    std::vector<Edge*> listEdgesAdd;

    for(auto it = originalMap.begin(), end = originalMap.end(); it!= end; it++)
    {
        Edge* e = network->getEdge(it->first, originalF->node_);
        if(e!=nullptr && !e->isFixed())
        {
            listEdgesRemove.push_back(e);
        }
    }

    int maxNRemove = (int)listEdgesRemove.size();
    int maxNAdd = (int)network->nodes_.size() - maxNRemove;

    for(auto it = network->nodes_.begin(), end = network->nodes_.end(); it!= end; it++)
    {
        bool isOriginalRegulator = false;
        for(auto it2 = originalMap.begin(), end2 = originalMap.end(); it2!=end2; it2++)
        {
            if(it->first.compare(it2->first) == 0)
            {
                isOriginalRegulator = true;
                break;
            }
        }
        if(!isOriginalRegulator)
        {
            Edge* newEdge = new Edge(it->second, originalN, 1);
            listEdgesAdd.push_back(newEdge);
        }
    }
    bool solFound = false;

    //iteration of number of add/remove operations
    for(int nOperations = 1; nOperations <= maxNRemove + maxNAdd; nOperations++)
    {

        for(int nAdd = 0; nAdd <= nOperations; nAdd++)
        {
            if(nAdd > maxNAdd)
            {
                break;
            }
            int nRemove = nOperations - nAdd;
            if(nRemove > maxNRemove)
            {
                continue;
            }

            if(nAdd == 0 && nRemove == 0)
            {
                continue;
            }
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: Testing " << nAdd << " adds and " << nRemove << " removes\n";

            std::vector<std::vector<Edge *>> listAddCombination = getEdgesCombinations(listEdgesAdd, nAdd);
            std::vector<std::vector<Edge *>> listRemoveCombination = getEdgesCombinations(listEdgesRemove, nRemove);
            std::vector<Edge *> emptyList;

            //only remove
            if(nAdd == 0)
            {

                for(auto itRemove = listRemoveCombination.begin(), endRemove = listRemoveCombination.end(); itRemove != endRemove; itRemove++)
                {
                    bool isSol = false;

                    //remove edges
                    for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                    {
                        network->removeEdge(*itRem);
                    }

                    //new function
                    Function * newF = new Function(originalN->id_, 0);
                    int clauseId = 1;
                    for(auto itReg = originalMap.begin(), endReg = originalMap.end(); itReg != endReg; itReg++)
                    {
                        bool removed = false;
                        for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                        {
                            if(itReg->first.compare((*itRem)->start_->id_) == 0)
                            {
                                removed = true;
                                break;
                            }
                        }
                        if(!removed)
                        {
                            newF->addElementClause(clauseId, itReg->first);
                            clauseId++;
                        }
                    }
                    originalN->addFunction(newF);

                    //test only functions
                    std::vector<Function*> fCandidates;

                    //test special case where all the edges are removed - node turned into input
                    if(clauseId == 1)
                    {
                        isSol = true;
                        RepairSet * repairSet = new RepairSet();
                        //remove edges in solution repair set
                        for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                        {
                            repairSet->removeEdge((*itRem));
                        }
                        repairSet->addRepairedFunction(newF);
                        solution->addRepairSet(iNode->id_, repairSet);
                    }
                    else
                    {
                        fCandidates.push_back(newF);
                    }
                    int bestFunctionLevel = -1;
                    while(!fCandidates.empty())
                    {
                        Function* candidate = fCandidates.front();
                        fCandidates.erase (fCandidates.begin());

                        if(bestFunctionLevel >= 0 && candidate->level_ > bestFunctionLevel)
                        {
                            //function is from a higher level than expected
                            continue;
                        }

                        if(isFuncConsistentWithLabel(solution, candidate))
                        {
                            isSol = true;
                        }

                        if(isSol)
                        {
                            RepairSet * repairSet = new RepairSet();

                            repairSet->addRepairedFunction(candidate);

                            //remove edges in solution repair set
                            for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                            {
                                repairSet->removeEdge((*itRem));
                            }

                            solution->addRepairSet(iNode->id_, repairSet);
                            bestFunctionLevel = candidate->level_;
                            if(!Configuration::isActive("allOpt"))
                            {
                                if(Configuration::isActive("debug"))
                                    std::cout << "DEBUG: no more solutions - allOpt\n";
                                break;;
                            }
                            
                        }
                        if(bestFunctionLevel >= 0 && !Configuration::isActive("showAllFunctions"))
                        {
                            if(Configuration::isActive("debug"))
                                std::cout << "DEBUG: no more function solutions\n";
                            break;           
                        }

                        if(candidate->getNumberOfRegulators() < 2)
                        {
                            if(Configuration::isActive("debug"))
                                std::cout << "DEBUG: function with 1 regulator\n";
                            //there is no possible condidates for 1 regulator function
                            break;
                        }
                        //renew candidates if solution level not found yet
                        if(bestFunctionLevel < 0 || candidate->level_ < bestFunctionLevel)
                        {
                            if(Configuration::isActive("debug"))
                                std::cout << "DEBUG: updating solutions\n";
                            std::vector<Function*> fauxCandidates = ASPHelper::getFunctionReplace(candidate, iNode->generalization_);
                            if(!fauxCandidates.empty())
                                fCandidates.insert(fCandidates.end(),fauxCandidates.begin(),fauxCandidates.end());
                        }

                    }

                    //test with edge flips
                    if(!isSol)
                        isSol = repairNodeConsistencyFlippingEdges(solution, iNode, emptyList, (*itRemove));


                    //add removed edges for the original network
                    for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                    {
                        network->addEdge((*itRem));
                    }

                    //put back the original function
                    originalN->addFunction(originalF);

                    if(isSol)
                    {
                        solFound = true;
                        if(!Configuration::isActive("allOpt"))
                        {
                            if(Configuration::isActive("debug"))
                                std::cout << "DEBUG: no more solutions - allOpt\n";
                            return;
                        }
                    }

                }
            }
            else
            {
                //only add
                if(nRemove == 0)
                {

                    for(auto itAdd = listAddCombination.begin(), endAdd = listAddCombination.end(); itAdd != endAdd; itAdd++)
                    {
                        bool isSol = false;

                        //add edges
                        for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                        {
                            network->addEdge(*itA);
                        }

                        //new function
                        Function * newF = new Function(originalN->id_, 0);
                        int clauseId = 1;
                        for(auto itReg = originalMap.begin(), endReg = originalMap.end(); itReg != endReg; itReg++)
                        {
                            newF->addElementClause(clauseId, itReg->first);
                            clauseId++;
                        }
                        for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                        {
                            newF->addElementClause(clauseId, (*itA)->start_->id_);
                            clauseId++;
                        }

                        originalN->addFunction(newF);

                        //test only functions
                        std::vector<Function*> fCandidates;
                        fCandidates.push_back(newF);
                        int bestFunctionLevel = -1;
                        while(!fCandidates.empty())
                        {
                            Function* candidate = fCandidates.front();
                            fCandidates.erase (fCandidates.begin());

                            if(bestFunctionLevel >= 0 && candidate->level_ > bestFunctionLevel)
                            {
                                //function is from a higher level than expected
                                continue;
                            }

                            if(isFuncConsistentWithLabel(solution, candidate))
                            {
                                isSol = true;
                            }


                            if(isSol)
                            {
                                RepairSet * repairSet = new RepairSet();

                                repairSet->addRepairedFunction(candidate);

                                //add edges in solution repair set
                                for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                                {
                                    repairSet->addEdge((*itA));
                                }

                                solution->addRepairSet(iNode->id_, repairSet);
                                bestFunctionLevel = candidate->level_;
                                if(!Configuration::isActive("allOpt"))
                                {
                                    if(Configuration::isActive("debug"))
                                        std::cout << "DEBUG: no more solutions - allOpt\n";
                                    break;;
                                }
                                
                            }
                            if(bestFunctionLevel >= 0 && !Configuration::isActive("showAllFunctions"))
                            {
                                if(Configuration::isActive("debug"))
                                    std::cout << "DEBUG: no more function solutions\n";
                                break;           
                            }

                            if(candidate->getNumberOfRegulators() < 2)
                            {
                                if(Configuration::isActive("debug"))
                                    std::cout << "DEBUG: function with 1 regulator\n";
                                //there is no possible condidates for 1 regulator function
                                break;
                            }
                            //renew candidates if solution level not found yet
                            if(bestFunctionLevel < 0 || candidate->level_ < bestFunctionLevel)
                            {
                                if(Configuration::isActive("debug"))
                                    std::cout << "DEBUG: updating solutions\n";
                                std::vector<Function*> fauxCandidates = ASPHelper::getFunctionReplace(candidate, iNode->generalization_);
                                if(!fauxCandidates.empty())
                                    fCandidates.insert(fCandidates.end(),fauxCandidates.begin(),fauxCandidates.end());
                            }

                        }

                        //test with edge flips
                        if(!isSol)
                            isSol = repairNodeConsistencyFlippingEdges(solution, iNode, (*itAdd), emptyList);

                        //remove added edges for the original network
                        for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                        {
                            network->removeEdge((*itA));
                        }

                        //put back the original function
                        originalN->addFunction(originalF);

                        if(isSol)
                        {
                            solFound = true;
                            if(!Configuration::isActive("allOpt"))
                            {
                                if(Configuration::isActive("debug"))
                                    std::cout << "DEBUG: no more solutions - allOpt\n";
                                return;
                            }
                        }

                    }

                }
                //both add and remove operations
                else
                {
                    for(auto itAdd = listAddCombination.begin(), endAdd = listAddCombination.end(); itAdd != endAdd; itAdd++)
                    {
                        for(auto itRemove = listRemoveCombination.begin(), endRemove = listRemoveCombination.end(); itRemove != endRemove; itRemove++)
                        {

                            bool isSol = false;

                            //remove and add edges
                            for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                            {
                                network->removeEdge(*itRem);
                            }
                            for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                            {
                                network->addEdge(*itA);
                            }

                            //new function
                            Function * newF = new Function(originalN->id_, 0);
                            int clauseId = 1;
                            for(auto itReg = originalMap.begin(), endReg = originalMap.end(); itReg != endReg; itReg++)
                            {
                                bool removed = false;
                                for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                                {
                                    if(itReg->first.compare((*itRem)->start_->id_) == 0)
                                    {
                                        removed = true;
                                        break;
                                    }
                                }
                                if(!removed)
                                {
                                    newF->addElementClause(clauseId, itReg->first);
                                    clauseId++;
                                }
                            }
                            for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                            {
                                newF->addElementClause(clauseId, (*itA)->start_->id_);
                                clauseId++;
                            }
                            originalN->addFunction(newF);

                            //test only functions
                            std::vector<Function*> fCandidates;
                            fCandidates.push_back(newF);
                            int bestFunctionLevel = -1;
                            while(!fCandidates.empty())
                            {
                                Function* candidate = fCandidates.front();
                                fCandidates.erase (fCandidates.begin());

                                if(bestFunctionLevel >= 0 && candidate->level_ > bestFunctionLevel)
                                {
                                    //function is from a higher level than expected
                                    continue;
                                }

                                if(isFuncConsistentWithLabel(solution, candidate))
                                {
                                    isSol = true;
                                }


                                if(isSol)
                                {
                                    RepairSet * repairSet = new RepairSet();

                                    repairSet->addRepairedFunction(candidate);

                                    //remove and add edges in solution repair set
                                    for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                                    {
                                        repairSet->removeEdge((*itRem));
                                    }
                                    for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                                    {
                                        repairSet->addEdge((*itA));
                                    }

                                    solution->addRepairSet(iNode->id_, repairSet);
                                    bestFunctionLevel = candidate->level_;
                                    if(!Configuration::isActive("allOpt"))
                                    {
                                        if(Configuration::isActive("debug"))
                                            std::cout << "DEBUG: no more solutions - allOpt\n";
                                        break;;
                                    }
                                    
                                }
                                if(bestFunctionLevel >= 0 && !Configuration::isActive("showAllFunctions"))
                                {
                                    if(Configuration::isActive("debug"))
                                        std::cout << "DEBUG: no more function solutions\n";
                                    break;           
                                }

                                if(candidate->getNumberOfRegulators() < 2)
                                {
                                    if(Configuration::isActive("debug"))
                                        std::cout << "DEBUG: function with 1 regulator\n";
                                    //there is no possible condidates for 1 regulator function
                                    break;
                                }
                                //renew candidates if solution level not found yet
                                if(bestFunctionLevel < 0 || candidate->level_ < bestFunctionLevel)
                                {
                                    if(Configuration::isActive("debug"))
                                        std::cout << "DEBUG: updating solutions\n";
                                    std::vector<Function*> fauxCandidates = ASPHelper::getFunctionReplace(candidate, iNode->generalization_);
                                    if(!fauxCandidates.empty())
                                        fCandidates.insert(fCandidates.end(),fauxCandidates.begin(),fauxCandidates.end());
                                }

                            }

                            //test with edge flips
                            if(!isSol)
                                isSol = repairNodeConsistencyFlippingEdges(solution, iNode, (*itAdd), (*itRemove));


                            //add and remove edges for the original network
                            for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                            {
                                network->addEdge((*itRem));
                            }
                            for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                            {
                                network->removeEdge((*itA));
                            }

                            //put back the original function
                            originalN->addFunction(originalF);

                            if(isSol)
                            {
                                solFound = true;
                                if(!Configuration::isActive("allOpt"))
                                {
                                    if(Configuration::isActive("debug"))
                                        std::cout << "DEBUG: no more solutions - allOpt\n";
                                    return;
                                }
                            }
                        }
                    }
                }

            }

        }

        if(solFound)
        {
            break;
        }

    }


    if(!solFound)
    {
        //TODO add or remove edges
        solution->hasImpossibility = true;
        std::cout << "WARN: Not possible to repair node " << iNode->id_ << std::endl;
    }
    return;
}


void testDouble(InconsistencySolution* inconsistency, InconsistentNode* iNode)
{

    std::vector<Function*> candidates;
    std::vector<Function*> consistentFunctions;


    //each function must have a list of replacement candidates and each must be tested until it works
    Function* originalF = network->getNode(iNode->id_)->getFunction();
    std::map<std::string,int> originalMap = originalF->getRegulatorsMap();
    if(originalF->getNumberOfRegulators() < 2)
    {
        repairNodeConsistencyWithTopologyChanges(inconsistency, iNode);
        return;
    }
    Function* newF = new Function(originalF->node_, 1);
    for(auto it = originalMap.begin(), end = originalMap.end(); it!= end; it++)
    {
        newF->addElementClause(1, it->first);
    }
    candidates.push_back(newF);

    std::cout << "Finding functions for double inconsistency in " << originalF->printFunction() << " (" << originalF->getNumberOfRegulators() << " regulators)\n\n";

    // get the possible candidates to replace the inconsistent function
    bool functionRepaired = false;
    int repairedFunctionLevel = -1;
    int counter=0;
    while(!candidates.empty())
    {
        counter++;
        Function* candidate = candidates.front();
        candidates.erase (candidates.begin());
        bool isConsistent = false;
        
        if(isIn(consistentFunctions, candidate))
        {
            continue;
        }

        if(isFuncConsistentWithLabel(inconsistency, candidate))
        {
            isConsistent = true;
            consistentFunctions.push_back(candidate);
            if(!functionRepaired)
                std::cout << "\tfound first function at level " << candidate->level_ << "  " << candidate->printFunction() << "\n";
            functionRepaired = true;
            if(repairedFunctionLevel == -1)
                repairedFunctionLevel = candidate->level_;
        }
        else
        {
            if(candidate->son_consistent)
                continue;
        }
        
        std::vector<Function*> tauxCandidates = ASPHelper::getFunctionReplace(candidate,true);
        for(auto it = tauxCandidates.begin(), end = tauxCandidates.end(); it!=end; it++)
        {
            (*it)->son_consistent = isConsistent;
            if(!isIn(candidates, (*it)))
                candidates.push_back((*it));
        }
        
    }
    if(functionRepaired)
    {
        std::cout << "\nPrinting consistent functions found\n\n";
        for(auto it = consistentFunctions.begin(), end = consistentFunctions.end(); it != end; it++)
        {
            std::cout << "\t" << (*it)->printFunction() << "(distance from bottom: " << (*it)->level_ << ")\n";
        }
    }
    else
    {
        std::cout << "no consistent functions found - " << counter << "\n";
    }

    return;

}

bool isIn(std::vector<Function*> list, Function* item)
{
    for(auto it = list.begin(), end = list.end(); it!= end; it++)
    {
        Function* aux = (*it);
        if(item->isEqual(aux))
            return true;
    }
    return false;
}
