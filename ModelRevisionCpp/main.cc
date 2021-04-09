#include "main.h"
#include "Configuration.h"
#include <iostream>
#include <fstream>
#include <string>
#include "ASPHelper.h"
#include "Network.h"
#include "Util.h"
#include <bitset>
#include <algorithm>
#include <math.h>

//same as used in InconsistencySolution and InconsistentNode
enum inconsistencies { CONSISTENT = 0, SINGLE_INC_GEN, SINGLE_INC_PART, DOUBLE_INC };
enum update_type { ASYNC = 0, SYNC, MASYNC};

std::string version = "1.3.0-unstable";

Network * network = new Network();
int update = ASYNC;

// verbose level
// 0 - machine style output (minimalistic easily parsable)
// 1 - machine style output (using set of sets)
// 2 - human readable output [default]
int verbose = 2;


int main(int argc, char ** argv) {

    Configuration::parseConfig(Util_h::getFilepath(argv[0]).append("config.txt"));

    if(process_arguments(argc, argv) != 0)
        return -1;

    int parse = ASPHelper::parseNetwork(network);
    if(parse < 1 && !Configuration::isActive("ignoreWarnings"))
    {
        std::cout << std::endl << "#ABORT:\tModel definition with errors." << std::endl << "\tCheck documentation for input definition details." << std::endl;
        return -1;
    }

    //main function that revises the model
    modelRevision();
    
}

// Function that initializes the program.
// Can process optional arguments or configurations
int process_arguments(const int argc, char const * const * argv) {

    if(argc < 2)
    {
        std::cout << "Invalid number of arguments: " << argc << std::endl;
        printHelp();
        return -1;
    }

    // observation type
    // 0 - time-series observations [default]
    // 1 - stable state observations
    // 2 - both (time-series + stable state)
    int obs_type = 0;

    std::string lastOpt = "-m";

    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if(strcmp(argv[i],"--sub-opt") == 0)
            {
                Configuration::setValue("showSolutionForEachInconsistency","true");
                continue;
            }
            if(strcmp(argv[i],"--exhaustive-search") == 0)
            {
                Configuration::setValue("forceOptimum","true");
                continue;
            }

            // retro-compatibiity option
            if(strcmp(argv[i],"--steady-state") == 0 || strcmp(argv[i],"--ss") == 0)
            {
                obs_type = 1;
                continue;
            }

            lastOpt = argv[i];
            if(lastOpt.compare("--help") == 0 || lastOpt.compare("-h") == 0)
            {
                printHelp();
                return 1;
            }
            
            if(lastOpt.compare("--model") != 0 && lastOpt.compare("-m") != 0 &&
                lastOpt.compare("--observations") != 0 && lastOpt.compare("-obs") != 0 &&
                lastOpt.compare("--observation-type") != 0 && lastOpt.compare("-ot") != 0 &&
                lastOpt.compare("--update") != 0 && lastOpt.compare("-up") != 0 &&
                lastOpt.compare("--verbose") != 0 && lastOpt.compare("-v") != 0)
            {
                std::cout << "Invalid option " << lastOpt << std::endl;
                printHelp();
                return -1;
            }
            
        }
        else
        {
            if(lastOpt.compare("--model") == 0 || lastOpt.compare("-m") == 0)
            {

                if(network->input_file_network_.empty())
                    network->input_file_network_ = argv[i];
                else
                    network->observation_files.push_back(argv[i]);
                continue;
            }
            if(lastOpt.compare("--observations") == 0 || lastOpt.compare("-obs") == 0)
            {
                network->observation_files.push_back(argv[i]);
                continue;
            }
            if(lastOpt.compare("--observation-type") == 0 || lastOpt.compare("-ot") == 0)
            {
                lastOpt = "-m";
                if(strcmp(argv[i], "ts") == 0)
                {
                    obs_type = 0;
                    continue;
                }
                if(strcmp(argv[i], "ss") == 0)
                {
                    obs_type = 1;
                    continue;
                }
                if(strcmp(argv[i], "both") == 0)
                {
                    obs_type = 2;
                    continue;
                }

                std::cout << "Invalid value for option --observation-type: " << argv[i] << std::endl;
                printHelp();
                return -1;
            }
            if(lastOpt.compare("--update") == 0 || lastOpt.compare("-up") == 0)
            {
                lastOpt = "-m";
                if(strcmp(argv[i], "a") == 0)
                {
                    update = ASYNC;
                    continue;
                }
                if(strcmp(argv[i], "s") == 0)
                {
                    update = SYNC;
                    continue;
                }
                if(strcmp(argv[i], "ma") == 0)
                {
                    update = MASYNC;
                    continue;
                }

                std::cout << "Invalid value for option --update: " << argv[i] << std::endl;
                printHelp();
                return -1;

            }
            if(lastOpt.compare("--verbose") == 0 || lastOpt.compare("-v") == 0)
            {
                lastOpt = "-m";
                try{
                    int value = std::stoi(argv[i]);
                    if(value >= 0 && value <= 2)
                    {
                        verbose = value;
                    }
                    else{
                        std::cout << "Invalid value for option --verbose: " << argv[i] << std::endl;
                        printHelp();
                        return -1;
                    }
                }
                catch(std::exception& e)
                {
                    std::cout << "Invalid value for option --verbose: " << argv[i] << std::endl;
                    printHelp();
                    return -1;
                }   
                continue;
            }

        }
    }

    if(obs_type == 0 || obs_type == 2)
    {
        network->has_ts_obs = true;
    }
    if(obs_type == 1 || obs_type == 2)
    {
        network->has_ss_obs = true;
    }

    return 0;
}

void printHelp()
{
    std::cout << "Model Revision program." << std::endl;
    std::cout << "  Given a model and a set of observations it determines if the model is consistent. If not, it computes all the minimum number of repair operations in order to render the model consistent." << std::endl;
    std::cout << "Version: " << version << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  modrev [-m] model_file [[-obs] observation_files...] [options]" << std::endl;
    std::cout << "  options:" << std::endl;
    std::cout << "    --model,-m <model_file>\t\tInput model file." << std::endl;
    std::cout << "    --observations,-obs <obs_files...>\tList of observation files." << std::endl;
    //std::cout << "\t\t--output,-o <output_file>\t\tOutput file destination." << std::endl;
    //std::cout << "    --stable-state,-ss <ss_files...>\tList of stable-state observation files" << std::endl;
    std::cout << "    --observation-type,-ot <value>\tType of observations in {ts|ss|both}. DEFAULT: ts." << std::endl;
    std::cout << "\t\t\t\t\t\tts   - time-series observations" << std::endl;
    std::cout << "\t\t\t\t\t\tss   - stable state observations" << std::endl;
    std::cout << "\t\t\t\t\t\tboth - both time-series and stable state observations" << std::endl;
    std::cout << "    --update,-up <value>\t\tUpdate mode in {a|s|ma}. DEFAULT: a." << std::endl;
    std::cout << "\t\t\t\t\t\ta  - asynchronous update" << std::endl;
    std::cout << "\t\t\t\t\t\ts  - synchronous update" << std::endl;
    std::cout << "\t\t\t\t\t\tma - multi-asynchronous update" << std::endl;
    std::cout << "    --exhaustive-search\t\t\tForce exhaustive search of function repair operations. DEFAULT: false." << std::endl;
    std::cout << "    --sub-opt\t\t\t\tShow sub-optimal solutions found. DEFAULT: false." << std::endl;
    std::cout << "    --verbose,-v <value>\t\tVerbose level {0,1,2} of output. DEFAULT: 2." << std::endl;
    std::cout << "\t\t\t\t\t\t0  - machine style output (minimalistic easily parsable)" << std::endl;
    std::cout << "\t\t\t\t\t\t1  - machine style output (using sets of sets)" << std::endl;
    std::cout << "\t\t\t\t\t\t2  - human readable output" << std::endl;
    std::cout << "    --help,-h\t\t\t\tPrint help options." << std::endl;

}

//Model revision procedure
// 1) tries to repair functions
// 2) tries to flip the sign of the edges
// 3) tries to add or remove edges
void modelRevision() {

    int optimization = -2;

    std::vector<InconsistencySolution*> fInconsistencies = checkConsistency(optimization);
    if(optimization <= 0)
    {
        return;
    }
    if(Configuration::isActive("debug"))
        std::cout << "found " << fInconsistencies.size() << " solutions with " << fInconsistencies.front()->iNodes_.size() << " inconsistent nodes" << std::endl;

    //At this point we have an inconsistent network with node candidates to be repaired

    //for each possible inconsistency solution/labelling, try to make the model consistent
    InconsistencySolution * bestSolution = nullptr;
    
    for(auto it = fInconsistencies.begin(), end = fInconsistencies.end(); it != end; it++)
    {

        repairInconsistencies((*it));

        //TODO
        if(!(*it)->hasImpossibility)
        {
            //if(bestSolution == nullptr || (*it)->getNTopologyChanges() < bestSolution->getNTopologyChanges())
            if(bestSolution == nullptr || (*it)->compareRepairs(bestSolution) > 0)
            {
                bestSolution = (*it);
                if(Configuration::isActive("debug"))
                    std::cout << "DEBUG: found solution with " << bestSolution->getNTopologyChanges() << " topology changes.\n";
                if(bestSolution->getNTopologyChanges() == 0 && !Configuration::isActive("allOpt"))
                    break;
            }
        }
        else
        {
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: Reached an impossibility\n";
        }
    }

    if(bestSolution == nullptr)
    {
        std::cout << "### It was not possible to repair the model." << std::endl;
        return;
    }
    
    bool showSubOpt = Configuration::isActive("showSolutionForEachInconsistency");
    //TODO
    if(Configuration::isActive("allOpt"))
    {
        //TODO: remove duplicates
        for(auto it = fInconsistencies.begin(), end = fInconsistencies.end(); it != end; it++)
        {
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: checking for printing solution with " << (*it)->getNTopologyChanges() << " topology changes\n";
            //if(!(*it)->hasImpossibility && (*it)->getNTopologyChanges() == bestSolution->getNTopologyChanges())
            if(!(*it)->hasImpossibility && ((*it)->compareRepairs(bestSolution) >= 0 || showSubOpt))
            {
                if(showSubOpt && (*it)->compareRepairs(bestSolution) < 0)
                {
                    if(verbose < 2)
                    {
                        std::cout << "+";
                    }
                    else
                    {
                        std::cout << "(Sub-Optimal Solution)\n";
                    }
                }
                (*it)->printSolution(verbose);
            }
        }

    }
    else
    {
        bestSolution->printSolution(verbose);
    }

}


//function reponsible to check the consistency of a model and return a set of possible function inconsistencies
std::vector<InconsistencySolution*> checkConsistency(int & optimization) {

    std::vector<InconsistencySolution*> result;
    
    //consistency check
    if(Configuration::isActive("check_ASP"))
    {
        // invoke the consistency check program in ASP
        result = ASPHelper::checkConsistency(network, optimization, update);
    }
    else
    {
        //TODO Add other implementations
        //convert ASP to sat or other representation
        //test consistency
    }

    if(optimization < 0)
    {
        std::cout << "It is not possible to repair this network for now." << std::endl;
    }

    if(optimization == 0)
    {
        std::cout << "This network is consistent!" << std::endl;
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
            if(Configuration::isActive("debug"))
                printf("#FOUND a node with impossibility - %s\n", it->second->id_.c_str());
            return;
        }
        
    }

}

//This function repair a given node and determines all possible solutions
//consider 0 .. N add/remove repair operations, STARTING with 0 repairs of this type!
void repairNodeConsistency(InconsistencySolution* inconsistency, InconsistentNode* iNode)
{
    Node * originalN = network->getNode(iNode->id_);
    Function * originalF = originalN->regFunction_;

    std::map<std::string,int> originalMap;
    if(originalF != nullptr)
        originalMap = originalF->getRegulatorsMap();
    std::vector<Edge*> listEdgesRemove;
    std::vector<Edge*> listEdgesAdd;

    for(auto it = originalMap.begin(), end = originalMap.end(); it!= end; it++)
    {
        Edge* e = network->getEdge(it->first, originalF->getNode());
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
    for(int nOperations = 0; nOperations <= maxNRemove + maxNAdd; nOperations++)
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
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: Testing " << nAdd << " adds and " << nRemove << " removes\n";
            

            std::vector<std::vector<Edge *>> listAddCombination = getEdgesCombinations(listEdgesAdd, nAdd);
            std::vector<std::vector<Edge *>> listRemoveCombination = getEdgesCombinations(listEdgesRemove, nRemove);

            for(auto itAdd = listAddCombination.begin(), endAdd = listAddCombination.end(); itAdd != endAdd; itAdd++)
            {
                for(auto itRemove = listRemoveCombination.begin(), endRemove = listRemoveCombination.end(); itRemove != endRemove; itRemove++)
                {

                    bool isSol = false;
                    //remove and add edges
                    for(auto itRem = (*itRemove).begin(), endRem = (*itRemove).end(); itRem != endRem; itRem++)
                    {
                        if(Configuration::isActive("debug"))
                            std::cout << "DEBUG: remove edge from " << (*itRem)->start_->id_ << "\n";
                        network->removeEdge(*itRem);
                    }
                    for(auto itA = (*itAdd).begin(), endA = (*itAdd).end(); itA != endA; itA++)
                    {
                        if(Configuration::isActive("debug"))
                            std::cout << "DEBUG: add edge from " << (*itA)->start_->id_ << "\n";
                        network->addEdge(*itA);
                    }

                    //if nOperations > 0 then the function must be changed
                    if(nOperations > 0)
                    {
                        //new function
                        Function * newF = new Function(originalN->id_);
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
                    }

                    //test with edge flips starting with 0 edge flips
                    isSol = repairNodeConsistencyFlippingEdges(inconsistency, iNode, (*itAdd), (*itRemove));

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

            //clean memory
            listAddCombination.clear();
            listRemoveCombination.clear();

        }

        if(solFound)
        {
            break;
        }

    }

    if(!solFound)
    {
        inconsistency->hasImpossibility = true;
        std::cout << "WARN: Not possible to repair node " << iNode->id_ << std::endl;
    }

    //clean memory
    listEdgesRemove.clear();
    listEdgesAdd.clear();
    listEdgesRemove.resize(0);
    listEdgesAdd.resize(0);


    return;

}


std::vector<std::vector<Edge *>> getEdgesCombinations(std::vector<Edge *> edges, int n)
{
    if(n == 0)
    {
        std::vector<Edge *> emptyVector;
        std::vector<std::vector<Edge *>> result;
        result.push_back(emptyVector);
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

//function that tries to repair the node by flipping the sign of edges
//starts with 0 edges and iteratively goes up
//return true if a solution is found
bool repairNodeConsistencyFlippingEdges(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges)
{
    Function * f = network->getNode(iNode->id_)->regFunction_;

    std::map<std::string,int> map;
    if(f != nullptr)
    {
        map = f->getRegulatorsMap();
    }
    std::vector<Edge*> listEdges;
    for(auto it = map.begin(), end = map.end(); it!= end; it++)
    {
        Edge* e = network->getEdge(it->first, f->getNode());
        if(e!=nullptr && !e->isFixed())
        {
            listEdges.push_back(e);
        }
    }

    if(Configuration::isActive("debug"))
        std::cout << "DEBUG: searching solution flipping edges for " << iNode->id_ << "\n";

    bool solFound = false;

    int iterations = (int)listEdges.size();
    //if a solution was already found,
    //do not exceed the number of flip edges from previous solutions
    if(iNode->repaired_)
    {
        iterations = iNode->getNFlipEdgesOperations();
    }

    for(int nEdges = 0; nEdges <= iterations; nEdges++)
    {
        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: testing with " << nEdges << " edge flips\n";
        std::vector<std::vector<Edge*>> eCandidates = getEdgesCombinations(listEdges, nEdges);

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
            bool isSol = repairNodeConsistencyFunctions(inconsistency, iNode, (*it), addedEdges, removedEdges);

            //put network back to normal
            for(auto itEdge = (*it).begin(), endEdge = (*it).end(); itEdge != endEdge; itEdge++)
            {
                Edge* e = (*itEdge);
                e->flipSign();
                if(Configuration::isActive("debug"))
                    std::cout << "DEBUG: return flip edge from " << e->start_->id_ << "\n";
            }
            if(isSol)
            {
                if(Configuration::isActive("debug"))
                    std::cout << "DEBUG: is solution by flipping edges\n";
                solFound = true;
                if(!Configuration::isActive("allOpt"))
                {
                    if(Configuration::isActive("debug"))
                        std::cout << "DEBUG: no more solutions - allOpt\n";
                    return true;
                }
            }

        }

        if(solFound)
        {
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: ready to end with " << nEdges << " edges flipped\n";
            break;
        }

        //clean memory
        eCandidates.clear();

    }

    return solFound;

}


//repairs the function of the node if necessary
bool repairNodeConsistencyFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges)
{
    bool solFound = false;

    int repairType = iNode->repairType;

    //if at least one topological operation was performed,
    //it is necessary to validate if the model became consistent
    if(!flippedEdges.empty() || !addedEdges.empty() || !removedEdges.empty())
    {
        repairType = nFuncInconsistentWithLabel(inconsistency, network->getNode(iNode->id_)->regFunction_);
        if(repairType == CONSISTENT)
        {
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: node consistent with only topological changes\n";
            RepairSet * repairSet = new RepairSet();
            for(auto itEdge = flippedEdges.begin(), endEdge = flippedEdges.end(); itEdge != endEdge; itEdge++)
            {
                Edge* e = (*itEdge);
                repairSet->addFlippedEdge(e);
            }
            //add and remove edges in solution repair set
            for(auto itRem = removedEdges.begin(), endRem = removedEdges.end(); itRem != endRem; itRem++)
            {
                repairSet->removeEdge((*itRem));
            }
            for(auto itAdd = addedEdges.begin(), endAdd = addedEdges.end(); itAdd != endAdd; itAdd++)
            {
                repairSet->addEdge((*itAdd));
            }
            if(!addedEdges.empty() || !removedEdges.empty())
            {
                repairSet->addRepairedFunction(network->getNode(iNode->id_)->regFunction_);
            }
            inconsistency->addRepairSet(iNode->id_, repairSet);
            return true;
        }
    }
    else
    {
        //no operation was performed yet and it is necessary to validate if is a topological change
        if(iNode->topologicalError_)
        {
            return false;
        }
    }
    if(repairType == CONSISTENT)
    {
        std::cout << "WARN: Found a consistent node before expected: " << iNode->id_ << std::endl;
    }

    //if a solution was already found,
    //do not exceed the number of operations from previous solutions
    //avoiding search for function when there is already a solutions without changing the function
    //with the same number of operations
    if(iNode->repaired_)
    {
        int nRAop = iNode->getNAddRemoveOperations();
        int nFEop = iNode->getNFlipEdgesOperations();
        int nOp = iNode->getNRepairOperations();
        if((nRAop == (int)addedEdges.size() + (int)removedEdges.size()) && (nFEop == (int)flippedEdges.size())
            && (nOp == nRAop + nFEop))
        {
            if(Configuration::isActive("debug"))
                std::cout << "DEBUG: better solution already found. No function search.\n";
            return false;
        }
    }

    //model not yet consistent and is necessary to change function
    if(repairType == DOUBLE_INC)
    {
        if(!addedEdges.empty() || !removedEdges.empty())
        {
            //if we have a double inconsistency and at least one edge was removed or added
            //means that the function was changed to the bottom function (disjunction of variables)
            //and it is not possible to repair the function
            return false;
        }

        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: searching for non comparable functions for node " << iNode->id_ << "\n";
        //case of double inconsistency
        solFound = searchNonComparableFunctions(inconsistency, iNode, flippedEdges, addedEdges, removedEdges);
        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: end searching for non comparable functions for node " << iNode->id_ << "\n";
    }
    else
    {
        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: searching for comparable functions for node " << iNode->id_ << "\n";
        //case of single inconsistency
        solFound = searchComparableFunctions(inconsistency, iNode, flippedEdges, addedEdges, removedEdges, repairType == SINGLE_INC_GEN);
        if(Configuration::isActive("debug"))
            std::cout << "DEBUG: end searching for comparable functions for node " << iNode->id_ << "\n";
    }

    return solFound;
}


int nFuncInconsistentWithLabel(InconsistencySolution* labeling, Function* f)
{
    int result = CONSISTENT;
    //verify for each profile
    for(auto it = labeling->vlabel_.begin(), end = labeling->vlabel_.end(); it != end; it++)
    {
        int ret = nFuncInconsistentWithLabel(labeling, f, it->first);
        //if(Configuration::isActive("debug"))
        //        std::cout << "DEBUG: consistency value: " << ret << " for node " << f->getNode() << " with function: " << f->printFunction() << std::endl;
        if(result == CONSISTENT)
        {
            result = ret;
        }
        else
        {
            if(ret != result && ret != CONSISTENT)
            {
                result = DOUBLE_INC;
                break;
            }
        }
    }
    return result;
}


int nFuncInconsistentWithLabel(InconsistencySolution* labeling, Function* f, std::string profile)
{
    if(Configuration::isActive("debug"))
        std::cout << "\n###DEBUG: checking consistency of function: " << f->printFunction() << " of node " << f->getNode() << "\n";

    int result = CONSISTENT;
    std::map<int, std::map<std::string, int> > * profileMap = &(labeling->vlabel_[profile]);
    //must test for each time step
    int time = 0;
    int lastVal = -1;
    bool isStableState = profileMap->size() == 1;
    if(Configuration::isActive("debug") && isStableState)
            std::cout << "DEBUG: testing inconsistencies in stable state profile: " << profile << "\n";

    while(profileMap->find(time) != profileMap->end())
    {
        if(Configuration::isActive("debug"))
            std::cout << "\t\tChecking a from time step: " << time << "\n";
        //if it is not steady state, the following time must exist
        if(!isStableState && profileMap->find(time + 1) == profileMap->end())
        {
            break;
        }

        std::map<std::string, int> * timeMap = &((*profileMap)[time]);

        //verify if it is an updated node
        if(!isStableState && update != SYNC)
        {
            std::vector<std::string> updates = labeling->updates_[time][profile];
            bool isUpdated = false;
            if(Configuration::isActive("debug"))
                std::cout << "\t\tTesting updates\n";
            for(auto it = updates.begin(), end = updates.end(); it != end; it++)
            {
                if(Configuration::isActive("debug"))
                        std::cout << "\t\tList of updated contains: " << (*it) << " and we are testing node " << f->getNode() << "\n";
                if((*it).compare(f->getNode())==0)
                {
                    isUpdated = true;
                    if(Configuration::isActive("debug"))
                        std::cout << "\t\tIs ASYNC and updated\n";
                    break;
                }
            }
            if(!isUpdated)
            {
                time++;
                if(Configuration::isActive("debug"))
                    std::cout << "\t\tIs AYNC but not updated\n";
                continue;
            }
        }

        bool foundSat = false;
        int nClauses = f->getNClauses();
        for(int i = 1; i <= nClauses; i++)
        {
            if(Configuration::isActive("debug"))
                std::cout << "\t\tChecking term " << i << "\n";
            bool isClauseSatisfiable = true;
            std::set<std::string> clause = f->getClauses()[i];
            for(auto it = clause.begin(), end = clause.end(); it!=end; it++)
            {
                Edge* e = network->getEdge((*it), f->getNode());
                if(e != nullptr)
                {
                    //positive interaction
                    if(e->getSign() > 0)
                    {
                        if((*timeMap)[(*it)] == 0)
                        {
                            isClauseSatisfiable = false;
                            break;
                        }
                    }
                    //negative interaction
                    else
                    {
                        if((*timeMap)[(*it)] > 0)
                        {
                            isClauseSatisfiable = false;
                            break;
                        }
                    }
                }
                else{
                    std::cout << "WARN: Missing edge from " << (*it) << " to " << f->getNode() << std::endl;
                    return false;
                }
            }
            if(isClauseSatisfiable)
            {
                foundSat = true;
                if(isStableState)
                {
                    if((*timeMap)[f->getNode()] == 1)
                    {
                        return CONSISTENT;
                    }
                    else
                    {
                        return SINGLE_INC_PART;
                    }
                }
                else
                {
                    if((*profileMap)[time+1][f->getNode()] != 1)
                    {
                        if(result == CONSISTENT || result == SINGLE_INC_PART)
                        {
                            result = SINGLE_INC_PART;
                        }
                        else
                        {
                            return DOUBLE_INC;
                        }
                        
                    }
                    break;

                }

            }

        }

        if(!foundSat)
        {
            if(isStableState)
            {
                if(nClauses == 0)
                {
                    return CONSISTENT;
                }
                else
                {
                    if((*timeMap)[f->getNode()] == 0)
                    {
                        return CONSISTENT;
                    }
                    return SINGLE_INC_GEN;
                }
            }
            else
            {
                if(nClauses == 0)
                {
                    //no function
                    //input node
                    if(lastVal < 0)
                    {
                        lastVal = (*timeMap)[f->getNode()];
                    }
                    if((*profileMap)[time+1][f->getNode()] != lastVal)
                    {
                        return DOUBLE_INC;
                    }

                }
                else
                {
                    if((*profileMap)[time+1][f->getNode()] != 0)
                    {
                        if(result == CONSISTENT || result == SINGLE_INC_GEN)
                        {
                            result = SINGLE_INC_GEN;
                        }
                        else
                        {
                            return DOUBLE_INC;
                        }
                    }
                }

            }
        }

        time++;
    }

    return result;
}


bool searchComparableFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges, bool generalize)
{
    bool solFound = false;

    //each function must have a list of replacement candidates and each must be tested until it works
    Function* originalF = network->getNode(iNode->id_)->getFunction();
    if(originalF == nullptr)
    {
        std::cout << "WARN: Inconsistent node " << iNode->id_ << " without regulatory function." << std::endl;
        inconsistency->hasImpossibility = true;
        return false;
    }

    if(originalF->getNumberOfRegulators() < 2)
    {
        return false;
    }

    if(Configuration::isActive("debug"))
        std::cout << "\tDEBUG: searching for comparable functions of dimension " << originalF->getNumberOfRegulators() << " going " << (generalize?"down":"up") << "\n";

    // get the possible candidates to replace the inconsistent function
    bool functionRepaired = false;
    int repairedFunctionLevel = -1;
    std::vector<Function*> tCandidates = originalF->getReplacements(generalize);
    while(!tCandidates.empty())
    {
        
        bool candidateSol = false;
        Function* candidate = tCandidates.front();
        tCandidates.erase (tCandidates.begin());

        //if(Configuration::isActive("debug"))
        //    std::cout << "\tDEBUG: testing function " << candidate->printFunction() << "\n";
        
        if(functionRepaired && candidate->distanceFromOriginal_ > repairedFunctionLevel)
        {
            //function is from a higher level than expected
            delete(candidate);
            continue;
        }

        if(isFuncConsistentWithLabel(inconsistency, candidate))
        {
            candidateSol = true;
            RepairSet * repairSet = new RepairSet();
            repairSet->addRepairedFunction(candidate);
            //add flipped edges in solution repair set
            for(auto itEdge = flippedEdges.begin(), endEdge = flippedEdges.end(); itEdge != endEdge; itEdge++)
            {
                repairSet->addFlippedEdge((*itEdge));
            }
            //add and remove edges in solution repair set
            for(auto itRem = removedEdges.begin(), endRem = removedEdges.end(); itRem != endRem; itRem++)
            {
                repairSet->removeEdge((*itRem));
            }
            for(auto itAdd = addedEdges.begin(), endAdd = addedEdges.end(); itAdd != endAdd; itAdd++)
            {
                repairSet->addEdge((*itAdd));
            }
            inconsistency->addRepairSet(iNode->id_, repairSet);
            functionRepaired = true;
            solFound = true;
            repairedFunctionLevel = candidate->distanceFromOriginal_;
            if(!Configuration::isActive("showAllFunctions"))
            {
                break;
            }
        }

        std::vector<Function*> tauxCandidates = candidate->getReplacements(generalize);
        if(!tauxCandidates.empty())
        {
            for(auto it = tauxCandidates.begin(), end = tauxCandidates.end(); it!=end; it++)
            {
                if(!isIn(tCandidates, (*it)))
                    tCandidates.push_back((*it));
            }
        }
        tauxCandidates.clear();
        tauxCandidates.resize(0);
        //tCandidates.insert(tCandidates.end(),tauxCandidates.begin(),tauxCandidates.end());
        if(!candidateSol)
        {
            delete(candidate);
        }
    }

    tCandidates.clear();
    tCandidates.resize(0);

    //try non comparable function if no function is found and optimum is required
    if(!solFound && Configuration::isActive("forceOptimum"))
    {
        return searchNonComparableFunctions(inconsistency, iNode, flippedEdges, addedEdges, removedEdges);
    }
    
    return solFound;

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
    if(Configuration::isActive("debug"))
        std::cout << "\n###DEBUG: checking consistency of function: " << f->printFunction() << " of node " << f->getNode() << "\n";

    std::map<int, std::map<std::string, int> > * profileMap = &(labeling->vlabel_[profile]);
    //must test for each time step
    int time = 0;
    bool isStableState = profileMap->size() == 1;
    if(Configuration::isActive("debug"))
        std::cout << "\t\tChecking a stable state profile: " << profile << "\n";
    int lastVal = -1;
    while(profileMap->find(time) != profileMap->end())
    {
        if(Configuration::isActive("debug"))
            std::cout << "\t\tChecking a from time step: " << time << "\n";
        //if it is not steady state, the following time must exist
        if(!isStableState && profileMap->find(time + 1) == profileMap->end())
        {
            break;
        }

        std::map<std::string, int> * timeMap = &((*profileMap)[time]);

        //verify if it is an updated node
        if(!isStableState && update != SYNC)
        {
            std::vector<std::string> updates = labeling->updates_[time][profile];
            bool isUpdated = false;
            for(auto it = updates.begin(), end = updates.end(); it != end; it++)
            {
                if((*it).compare(f->getNode())==0)
                {
                    isUpdated = true;
                    if(Configuration::isActive("debug"))
                        std::cout << "\t\tIs ASYNC and updated\n";
                    break;
                }
            }
            if(!isUpdated)
            {
                time++;
                if(Configuration::isActive("debug"))
                    std::cout << "\t\tIs AYNC but not updated\n";
                continue;
            }
        }

        bool foundSat = false;
        int nClauses = f->getNClauses();
        for(int i = 1; i <= nClauses; i++)
        {
            if(Configuration::isActive("debug"))
                std::cout << "\t\tChecking term " << i << "\n";
            bool isClauseSatisfiable = true;
            std::set<std::string> clause = f->getClauses()[i];
            for(auto it = clause.begin(), end = clause.end(); it!=end; it++)
            {
                Edge* e = network->getEdge((*it), f->getNode());
                if(e != nullptr)
                {
                    //positive interaction
                    if(e->getSign() > 0)
                    {
                        if((*timeMap)[(*it)] == 0)
                        {
                            isClauseSatisfiable = false;
                            break;
                        }
                    }
                    //negative interaction
                    else
                    {
                        if((*timeMap)[(*it)] > 0)
                        {
                            isClauseSatisfiable = false;
                            break;
                        }
                    }
                }
                else{
                    std::cout << "WARN: Missing edge from " << (*it) << " to " << f->getNode() << std::endl;
                    return false;
                }
            }
            if(isClauseSatisfiable)
            {
                foundSat = true;
                if(isStableState)
                {
                    if((*timeMap)[f->getNode()] == 1)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    if((*profileMap)[time+1][f->getNode()] != 1)
                    {
                        return false;
                        
                    }
                    break;

                }

            }

        }

        if(!foundSat)
        {
            if(isStableState)
            {
                if(nClauses == 0)
                {
                    return true;
                }
                if((*timeMap)[f->getNode()] == 0)
                {
                    return true;
                }
                return false;
            }
            else
            {
                if(nClauses == 0)
                {
                    //no function
                    //input node
                    if(lastVal < 0)
                    {
                        lastVal = (*timeMap)[f->getNode()];
                    }
                    if((*profileMap)[time+1][f->getNode()] != lastVal)
                    {
                        return false;
                    }

                }
                else
                {
                    if((*profileMap)[time+1][f->getNode()] != 0)
                    {
                        return false;
                    }
                }

            }
        }

        time++;
    }

    return true;
}


bool searchNonComparableFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges)
{
    bool solFound = false;
    std::vector<Function*> candidates;
    std::vector<Function*> consistentFunctions;

    //to find the best possible functions comparing the levels
    bool levelCompare = Configuration::isActive("compareLevelFunction");
    std::vector<Function*> bestBelow;
    std::vector<Function*> bestAbove;
    std::vector<Function*> equalLevel;


    //each function must have a list of replacement candidates and each must be tested until it works
    Function* originalF = network->getNode(iNode->id_)->getFunction();
    std::map<std::string,int> originalMap = originalF->getRegulatorsMap();
    if(originalF->getNumberOfRegulators() < 2)
    {
        return false;
    }
    if(Configuration::isActive("debug"))
            std::cout << "\tDEBUG: searching for non comparable functions of dimension " << originalF->getNumberOfRegulators() << "\n";

    //construction of new function to start search
    //consider the nearest extreme
    Function* newF = new Function(originalF->getNode());

    //if the function is in the lower half of the Hasse diagram,
    //start search at the most specific function and generalize
    //else start at the most generic function and specify
    bool isGeneralize = true;
    if(levelCompare)
    {
        if(Configuration::isActive("debug"))
            printf("DEBUG: Starting half determination\n");
        isGeneralize = isFunctionInBottomHalf(originalF);
        if(Configuration::isActive("debug"))
            printf("DEBUG: End half determination\n");
        if(Configuration::isActive("debug"))
            printf("DEBUG: Performing a search going %s\n", isGeneralize ? "up" : "down");
    }
    int cindex = 1;
    for(auto it = originalMap.begin(), end = originalMap.end(); it!= end; it++)
    {
        newF->addElementClause(cindex, it->first);
        if(!isGeneralize)
        {
            cindex++;
        }

    }

    candidates.push_back(newF);
    if(Configuration::isActive("debug"))
        printf("DEBUG: Finding functions for double inconsistency in %s %s (%d regulators)\n\n",originalF->printFunction().c_str(), originalF->printFunctionFullLevel().c_str(), originalF->getNumberOfRegulators());

    // get the possible candidates to replace the inconsistent function
    bool functionRepaired = false;
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
        int incType = nFuncInconsistentWithLabel(inconsistency, candidate);
        if(incType == CONSISTENT)
        {
            isConsistent = true;
            consistentFunctions.push_back(candidate);
            if(!functionRepaired && Configuration::isActive("debug"))
                printf("\tDEBUG: found first function at level %d %s\n",candidate->distanceFromOriginal_, candidate->printFunction().c_str());
            functionRepaired = true;
            solFound = true;
            
            if(levelCompare)
            {
                int cmp = originalF->compareLevel(candidate);
                if(cmp == 0)
                {
                    //same level function
                    equalLevel.push_back(candidate);
                    continue;
                }
                if(isGeneralize && cmp < 0 && !equalLevel.empty())
                {
                    //if the candidate is above the original and we are going up and we found an equal level function
                    //then do nothing - do not continue explore this branch
                    continue;
                }
                if(!isGeneralize && cmp > 0 && !equalLevel.empty())
                {
                    //if the candidate is below the original and we are going down and we found an equal level function
                    //then do nothing - do not continue explore this branch
                    continue;
                }
                if(cmp > 0 && equalLevel.empty())
                {
                    //candidate below original function
                    if(bestBelow.empty())
                    {
                        bestBelow.push_back(candidate);
                    }
                    else
                    {
                        Function * representant = bestBelow.front();
                        int repCmp = representant->compareLevel(candidate);
                        if(repCmp == 0)
                        {
                            bestBelow.push_back(candidate);
                        }
                        if(repCmp < 0)
                        {
                            bestBelow.clear();
                            bestBelow.push_back(candidate);
                        }
                    }
                    if(!isGeneralize)
                    {
                        //if we are going down and found a below function, do not continue explore this branch
                        continue;
                    }
                }
                if(cmp < 0 && equalLevel.empty())
                {
                    //candidate above the original function
                    if(bestAbove.empty())
                    {
                        bestAbove.push_back(candidate);
                    }
                    else
                    {
                        Function * representant = bestAbove.front();
                        int repCmp = representant->compareLevel(candidate);
                        if(repCmp == 0)
                        {
                            bestAbove.push_back(candidate);
                        }
                        if(repCmp > 0)
                        {
                            bestAbove.clear();
                            bestAbove.push_back(candidate);
                        }
                        
                    }
                    if(isGeneralize)
                    {
                         //if we are going up and found an above function, do not continue explore this branch
                        continue;
                    }
                }
            }

        }
        //not consistent candidate
        else
        {
            //if the function is inconsistent but it came from a consistent function
            //then we can stop search its branch further
            if(candidate->son_consistent)
            {
                delete(candidate);
                continue;
            }
            
            //if the function is not consistent and has a inconsistency diferent from the direction we are going
            // then we can stop exploring such branch
            if(incType == DOUBLE_INC || (isGeneralize && incType == SINGLE_INC_PART) || (!isGeneralize && incType == SINGLE_INC_GEN))
            {
                delete(candidate);
                continue;
            }

            if(levelCompare)
            {
                if(isGeneralize && !equalLevel.empty() && candidate->compareLevel(originalF) > 0)
                {
                    delete(candidate);
                    continue;
                }
                if(!isGeneralize && !equalLevel.empty() && candidate->compareLevel(originalF) < 0)
                {
                    delete(candidate);
                    continue;
                }
                if(isGeneralize && !bestAbove.empty())
                {
                    Function* representant = bestAbove.front();
                    int repCmp = representant->compareLevel(candidate);
                    if(repCmp < 0)
                    {
                        delete(candidate);
                        continue;
                    }
                }
                if(!isGeneralize && !bestBelow.empty())
                {
                    Function* representant = bestBelow.front();
                    int repCmp = representant->compareLevel(candidate);
                    if(repCmp > 0)
                    {
                        delete(candidate);
                        continue;
                    }
                }
            }
        }
        
        std::vector<Function*> tauxCandidates = candidate->getReplacements(isGeneralize);
        for(auto it = tauxCandidates.begin(), end = tauxCandidates.end(); it!=end; it++)
        {
            (*it)->son_consistent = isConsistent;
            if(!isIn(candidates, (*it)))
                candidates.push_back((*it));
        }
        if(!isConsistent)
        {
            delete(candidate);
        }

        tauxCandidates.clear();
        tauxCandidates.resize(0);
        
    }
    if(Configuration::isActive("debug"))
    {
        if(functionRepaired)
        {
            if(levelCompare)
            {
                printf("\nDEBUG: Printing consistent functions found using level comparison\n");
                if(!equalLevel.empty())
                {
                    printf("Looked at %d functions. Found %d consistent. To return %d functions of same level\n\n", counter, (int)consistentFunctions.size(), (int)equalLevel.size());
                    for(auto it = equalLevel.begin(), end = equalLevel.end(); it != end; it++)
                    {
                        printf("\t %s %s (distance from bottom: %d)\n", (*it)->printFunction().c_str(), (*it)->printFunctionFullLevel().c_str(), (*it)->distanceFromOriginal_);
                    }
                }
                else
                {
                    printf("Looked at %d functions. Found %d consistent. To return %d functions\n\n", counter, (int)consistentFunctions.size(), (int)bestBelow.size()+(int)bestAbove.size());
                    for(auto it = bestBelow.begin(), end = bestBelow.end(); it != end; it++)
                    {
                        printf("\t %s %s (distance from bottom: %d)\n", (*it)->printFunction().c_str(), (*it)->printFunctionFullLevel().c_str(), (*it)->distanceFromOriginal_);
                    }
                    for(auto it = bestAbove.begin(), end = bestAbove.end(); it != end; it++)
                    {
                        printf("\t %s %s (distance from bottom: %d)\n", (*it)->printFunction().c_str(), (*it)->printFunctionFullLevel().c_str(), (*it)->distanceFromOriginal_);
                    }
                }
                
            }
            else
            {
                //printf("\nPrinting consistent functions found\n");
                printf("DEBUG: Looked at %d functions. Found %d functions\n\n", counter, (int)consistentFunctions.size());
                //for(auto it = consistentFunctions.begin(), end = consistentFunctions.end(); it != end; it++)
                //{
                //    printf("\t %s %s (distance from bottom: %d)\n", (*it)->printFunction().c_str(), (*it)->printFunctionFullLevel().c_str(), (*it)->level_);
                //}
            }
            
        }
        else
        {
            printf("DEBUG: no consistent functions found - %d\n", counter);
        }
    }

    //add repair sets to the solution
    if(solFound)
    {
        if(levelCompare)
        {
            if(!equalLevel.empty())
            {
                for(auto it = equalLevel.begin(), end = equalLevel.end(); it != end; it++)
                {
                    RepairSet * repairSet = new RepairSet();
                    repairSet->addRepairedFunction((*it));

                    //add flipped edges in solution repair set
                    for(auto itEdge = flippedEdges.begin(), endEdge = flippedEdges.end(); itEdge != endEdge; itEdge++)
                    {
                        repairSet->addFlippedEdge((*itEdge));
                    }
                    //add and remove edges in solution repair set
                    for(auto itRem = removedEdges.begin(), endRem = removedEdges.end(); itRem != endRem; itRem++)
                    {
                        repairSet->removeEdge((*itRem));
                    }
                    for(auto itAdd = addedEdges.begin(), endAdd = addedEdges.end(); itAdd != endAdd; itAdd++)
                    {
                        repairSet->addEdge((*itAdd));
                    }
                    inconsistency->addRepairSet(iNode->id_, repairSet);
                }
            }
            else
            {
                for(auto it = bestBelow.begin(), end = bestBelow.end(); it != end; it++)
                {
                    RepairSet * repairSet = new RepairSet();
                    repairSet->addRepairedFunction((*it));

                    //add flipped edges in solution repair set
                    for(auto itEdge = flippedEdges.begin(), endEdge = flippedEdges.end(); itEdge != endEdge; itEdge++)
                    {
                        repairSet->addFlippedEdge((*itEdge));
                    }
                    //add and remove edges in solution repair set
                    for(auto itRem = removedEdges.begin(), endRem = removedEdges.end(); itRem != endRem; itRem++)
                    {
                        repairSet->removeEdge((*itRem));
                    }
                    for(auto itAdd = addedEdges.begin(), endAdd = addedEdges.end(); itAdd != endAdd; itAdd++)
                    {
                        repairSet->addEdge((*itAdd));
                    }
                    inconsistency->addRepairSet(iNode->id_, repairSet);
                }
                for(auto it = bestAbove.begin(), end = bestAbove.end(); it != end; it++)
                {
                    RepairSet * repairSet = new RepairSet();
                    repairSet->addRepairedFunction((*it));

                    //add flipped edges in solution repair set
                    for(auto itEdge = flippedEdges.begin(), endEdge = flippedEdges.end(); itEdge != endEdge; itEdge++)
                    {
                        repairSet->addFlippedEdge((*itEdge));
                    }
                    //add and remove edges in solution repair set
                    for(auto itRem = removedEdges.begin(), endRem = removedEdges.end(); itRem != endRem; itRem++)
                    {
                        repairSet->removeEdge((*itRem));
                    }
                    for(auto itAdd = addedEdges.begin(), endAdd = addedEdges.end(); itAdd != endAdd; itAdd++)
                    {
                        repairSet->addEdge((*itAdd));
                    }
                    inconsistency->addRepairSet(iNode->id_, repairSet);
                }
            }
        }
        else
        {
            for(auto it = consistentFunctions.begin(), end = consistentFunctions.end(); it != end; it++)
            {
                RepairSet * repairSet = new RepairSet();
                repairSet->addRepairedFunction((*it));

                //add flipped edges in solution repair set
                for(auto itEdge = flippedEdges.begin(), endEdge = flippedEdges.end(); itEdge != endEdge; itEdge++)
                {
                    repairSet->addFlippedEdge((*itEdge));
                }
                //add and remove edges in solution repair set
                for(auto itRem = removedEdges.begin(), endRem = removedEdges.end(); itRem != endRem; itRem++)
                {
                    repairSet->removeEdge((*itRem));
                }
                for(auto itAdd = addedEdges.begin(), endAdd = addedEdges.end(); itAdd != endAdd; itAdd++)
                {
                    repairSet->addEdge((*itAdd));
                }
                inconsistency->addRepairSet(iNode->id_, repairSet);
            }
        }

    }

    candidates.clear();
    candidates.resize(0);
    consistentFunctions.clear();
    consistentFunctions.resize(0);
    bestBelow.clear();
    bestBelow.resize(0);
    bestAbove.clear();
    bestAbove.resize(0);
    equalLevel.clear();
    equalLevel.resize(0);
    
    return solFound;

}


bool isFunctionInBottomHalf(Function *f)
{
    if(Configuration::isActive("exactMiddleFunctionDetermination"))
    {
        if(Configuration::isActive("debug"))
            printf("DEBUG: Half determination by state\n");
        return isFunctionInBottomHalfByState(f);
    }
    int n = f->getNumberOfRegulators();
    int n2 = n/2;
    std::vector<int> midLevel;
    for(int i = 0; i < n; i++)
    {
        midLevel.push_back(n2);
    }
    return f->compareLevel(midLevel) < 0;

}


bool isFunctionInBottomHalfByState(Function *f)
{
    std::map<std::string,int> regMap = f->getRegulatorsMap();
    int regulators = f->getNumberOfRegulators();
    int entries = (int) pow(2, regulators);
    int nOne = 0;
    int nZero = 0;

    for(int entry = 0; entry < entries; entry++)
    {
        std::bitset<16> bits (entry);
        std::map<std::string,int> input;
        int bitIndex = 0;
        for(auto it = regMap.begin(), end = regMap.end(); it!=end; it++)
        {
            input.insert(std::pair<std::string, int>(it->first, bits.test(bitIndex) ? 1 : 0));
            bitIndex++;
        }
        if(getFunctionValue(f, input))
        {
            nOne++;
            if(nOne > entries / 2)
                break;
        }
        else
        {
            nZero++;
            if(nZero > entries / 2)
                break;
        }

    }
    if(nZero > entries / 2)
    {
        return true;
    }
    return false;

}

bool getFunctionValue(Function * f, std::map<std::string,int> input)
{
    int nClauses = f->getNClauses();
    for(int i = 1; i <= nClauses; i++)
    {
        bool isClauseSatisfiable = true;
        std::set<std::string> clause = f->getClauses()[i];
        for(auto it = clause.begin(), end = clause.end(); it!=end; it++)
        {
            Edge* e = network->getEdge((*it), f->getNode());
            if(e != nullptr)
            {
                //positive interaction
                if(e->getSign() > 0)
                {
                    if(input[(*it)] == 0)
                    {
                        isClauseSatisfiable = false;
                        break;
                    }
                }
                //negative interaction
                else
                {
                    if(input[(*it)] > 0)
                    {
                        isClauseSatisfiable = false;
                        break;
                    }
                }
            }
            else{
                std::cout << "WARN: Missing edge from " << (*it) << " to " << f->getNode() << std::endl;
                return false;
            }
        }
        if(isClauseSatisfiable)
            return true;

    }
    return false;
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