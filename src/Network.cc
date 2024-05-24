#include "Network.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Configuration.h"
#include <BooleanFunction.h>
#include <set>
#include "Util.h"

Network::Network() 
    :nodes_(),
    edges_(),
    observation_files(){
        has_ss_obs = false;
        has_ts_obs = false;
    }

Network::~Network() {

    for(auto it = nodes_.begin(), end = nodes_.end(); it != end; it++)
    {
        delete it->second;
    }

    for(auto it = edges_.begin(), end = edges_.end(); it != end; it++)
    {
        delete (*it);
    }
}

std::vector< Node* > Network::getNodes() {

    std::vector< Node* > result;
    for(auto it = nodes_.begin(), end = nodes_.end(); it != end; it++)
    {
        result.push_back(it->second);
    }
    return result;

}

Node* Network::addNode(std::string id) {
    Node* node = new Node(id);
    auto ret = nodes_.insert(std::make_pair(id, node));
    if(ret.second == false)
    {
        delete node;
        return ret.first->second;
    }
    return node;
}

Node* Network::getNode(std::string id) {
    auto it = nodes_.find(id);
    if(it != nodes_.end())
        return it->second;
    return nullptr;
}

std::vector< Edge* > Network::getEdges() {
    return edges_;
}

Edge* Network::addEdge(Node* start, Node* end, int sign) {
    Edge* edge = new Edge(start, end, sign);
    edges_.push_back(edge);
    return edge;
}

Edge* Network::getEdge(std::string startNode, std::string endNode)
{
    for(auto it = edges_.begin(), end = edges_.end(); it!=end; it++)
    {
        if((*it)->getStart()->id_.compare(startNode) == 0 && (*it)->getEnd()->id_.compare(endNode) == 0)
            return (*it);
    }
    return nullptr;
}

void Network::removeEdge(std::string startNode, std::string endNode)
{
    for(auto it = edges_.begin(), end = edges_.end(); it!=end; it++)
    {
        if((*it)->getStart()->id_.compare(startNode) == 0 && (*it)->getEnd()->id_.compare(endNode) == 0)
        {
            edges_.erase(it);
            return;
        }
    }
    return;
}

void Network::removeEdge(Edge* e)
{
    for(auto it = edges_.begin(), end = edges_.end(); it!=end; it++)
    {
        if((*it)->getStart()->id_.compare(e->start_->id_) == 0 && (*it)->getEnd()->id_.compare(e->end_->id_) == 0)
        {
            edges_.erase(it);
            return;
        }
    }
}

void Network::addEdge(Edge * e) {
    edges_.push_back(e);
    return;
}



Edge::Edge(Node* start, Node* end, int sign)
    :start_(start),
    end_(end),
    sign_(sign){
        fixed_ = false;
    }

Edge::~Edge() {}

Node* Edge::getStart() {
    return start_;
}

Node* Edge::getEnd() {
    return end_;
}

int Edge::getSign() {
    return sign_;
}

void Edge::flipSign(){
    if(sign_ == 0)
        sign_ = 1;
    else
        sign_ = 0;
}

bool Edge::isFixed(){
    return fixed_;
}

void Edge::setFixed(){
    fixed_ = true;
}

bool Edge::isEqual(Edge* e, bool checkSign)
{
    if(start_->id_.compare(e->getStart()->id_) != 0 || end_->id_.compare(e->getEnd()->id_) != 0)
    {
        return false;
    }
    if(checkSign)
    {
        return sign_ == e->getSign();
    }
    return true;
}


Node::Node(std::string id)
    :id_(id)
    {
        regFunction_ = new Function(id);
    }

Node::~Node()
{
    if(regFunction_ != nullptr)
        delete regFunction_;
}

Function* Node::addFunction(Function* regulation) {
    regFunction_ = regulation;
    return regulation;
}

Function* Node::getFunction() {
    return regFunction_;
}


Function::Function(std::string node)
{
    booleanF_ = new BooleanFunction::Function(node);
    distanceFromOriginal_ = 0;
    son_consistent = false;
}

Function::Function(BooleanFunction::Function * f)
{
    booleanF_ = f;
    distanceFromOriginal_ = 0;
    son_consistent = false;
}

Function::~Function() {
   delete(booleanF_); 
}

std::string Function::getNode()
{
    return booleanF_->getOutputVariable();
}

void Function::addElementClause(int id, std::string node) {
    booleanF_->addVariableToTerm(id, node);
}


int Function::getNumberOfRegulators(){
    return booleanF_->getDimension();
}


std::map<std::string,int> Function::getRegulatorsMap(){
    return booleanF_->getVariableMap();
}

std::map<int, std::set<std::string>> Function::getClauses(){
    return booleanF_->getTerms();
}

int Function::getNClauses()
{
    return booleanF_->getNTerms();
}

std::string Function::printFunction(){
    return BooleanFunction::PrintFunction(booleanF_);
}

bool Function::isEqual(Function* f)
{
    return booleanF_->isEqual(f->getBooleanFunction());
}

std::vector<int> Function::getFullLevel()
{
    return booleanF_->getLevel();
}

//returns -1 if the original function is lower
//return 0 if the functions are the same level
//return 1 if the other function is lower
int Function::compareLevel(Function * f)
{
    return booleanF_->compareLevel(f->getBooleanFunction());
}

int Function::compareLevel(std::vector<int> otherLevel)
{
    return booleanF_->compareLevel(otherLevel);
}



std::string Function::printFunctionFullLevel()
{
    return BooleanFunction::PrintFunctionLevel(booleanF_);
}

std::vector<Function*> Function::getParents()
{
    std::vector<Function*> result;
    std::vector<BooleanFunction::Function*> parents = booleanF_->getParents();
    for(auto it = parents.begin(), end = parents.end(); it != end; it++)
    {
        Function * f = new Function((*it));
        f->distanceFromOriginal_ = distanceFromOriginal_ + 1;
        result.push_back(f);
    }
    return result;
}

std::vector<Function*> Function::getChildren()
{
    std::vector<Function*> result;
    std::vector<BooleanFunction::Function*> parents = booleanF_->getChildren();
    for(auto it = parents.begin(), end = parents.end(); it != end; it++)
    {
        Function * f = new Function((*it));
        f->distanceFromOriginal_ = distanceFromOriginal_ + 1;
        result.push_back(f);
    }
    return result;
}

std::vector<Function*> Function::getReplacements(bool generalize)
{
    if(generalize)
    {
        return getParents();
    }
    return getChildren();
}

BooleanFunction::Function * Function::getBooleanFunction()
{
    return booleanF_;
}


InconsistencySolution::InconsistencySolution()
    :iNodes_(),
    vlabel_(),
    updates_(),
    iProfiles_(),
    iNodesProfiles_() {
        nTopologyChanges_ = 0;
        nAROperations = 0;
        nEOperations = 0;
        nRepairOperations_ = 0;
        hasImpossibility = false;
    }

InconsistencySolution::~InconsistencySolution(){}

void InconsistencySolution::addGeneralization(std::string id) {

    auto it = iNodes_.find(id);
    if ( it == iNodes_.end() ) {
        InconsistentNode* newINode = new InconsistentNode(id, true);
        iNodes_.insert(std::make_pair(id, newINode));
    } else {
        if(it->second->repairType != 1)
        {
            if(it->second->repairType == 0)
            {
                it->second->repairType = 1;
            }
            else
            {
                it->second->repairType = 3;
            }
        }
    }
    

}


void InconsistencySolution::addParticularization(std::string id) {

    auto it = iNodes_.find(id);
    if ( it == iNodes_.end() ) {
        InconsistentNode* newINode = new InconsistentNode(id, false);
        iNodes_.insert(std::make_pair(id, newINode));
    } else {
        if(it->second->repairType != 2)
        {
            if(it->second->repairType == 0)
            {
                it->second->repairType = 2;
            }
            else
            {
                it->second->repairType = 3;
            }
        }
    }

}

void InconsistencySolution::addTopologicalError(std::string id)
{
    auto it = iNodes_.find(id);
    if ( it == iNodes_.end() ) {
        InconsistentNode* newINode = new InconsistentNode(id, false);
        newINode->repairType = 0;
        newINode->topologicalError_ = true;
        iNodes_.insert(std::make_pair(id, newINode));
    } else {
        it->second->topologicalError_ = true;
    }
}




void InconsistencySolution::addVLabel(std::string profile, std::string id, int value, int time) {
    if(vlabel_.find(profile) == vlabel_.end())
    {
        std::map<int, std::map<std::string, int> > newMap;
        vlabel_.insert(std::make_pair(profile, newMap));
    }
    std::map<int, std::map<std::string, int> > * profileMap = &(vlabel_.find(profile)->second);

    if(profileMap->find(time) == profileMap->end())
    {
        std::map<std::string, int> newMap;
        profileMap->insert(std::make_pair(time, newMap));
    }

    profileMap->find(time)->second.insert(std::make_pair(id, value));
}

void InconsistencySolution::addUpdate(int time, std::string profile, std::string id)
{
    if(updates_.find(time) == updates_.end())
    {
        std::map<std::string, std::vector<std::string> > newMap;
        updates_.insert(std::make_pair(time, newMap));
    }

    std::map<std::string, std::vector<std::string> > * timeMap = &(updates_.find(time)->second);

    if(timeMap->find(profile) == timeMap->end())
    {
        std::vector<std::string> newVector;
        timeMap->insert(std::make_pair(profile, newVector));
    }

    timeMap->find(profile)->second.push_back(id);
}

void InconsistencySolution::addInconsistentProfile(std::string profile, std::string id)
{
    if(iProfiles_.find(profile) == iProfiles_.end())
    {
        std::vector<std::string> newVector;
        iProfiles_.insert(std::make_pair(profile, newVector));
    }
    iProfiles_.find(profile)->second.push_back(id);

    if(iNodesProfiles_.find(id) == iNodesProfiles_.end())
    {
        std::vector<std::string> newVector;
        iNodesProfiles_.insert(std::make_pair(id, newVector));
    }
    iNodesProfiles_.find(id)->second.push_back(profile);

}

void InconsistencySolution::addRepairSet(std::string id, RepairSet* repairSet)
{
    auto target = iNodes_.find(id);
    if(target != iNodes_.end())
    {
        if(!target->second->repaired_)
        {
            nTopologyChanges_ += repairSet->getNTopologyChanges();
            nAROperations += repairSet->getNAddRemoveOperations();
            nEOperations += repairSet->getNFlipEdgesOperations();
            nRepairOperations_ += repairSet->getNRepairOperations();
        }
        else
        {
            //filter solutions not optimal
            if(repairSet->getNAddRemoveOperations() > target->second->getNAddRemoveOperations())
            {
                return;
            }
            if(repairSet->getNAddRemoveOperations() == target->second->getNAddRemoveOperations() && 
                repairSet->getNFlipEdgesOperations() > target->second->getNFlipEdgesOperations())
            {
                return;
            }
            if(repairSet->getNAddRemoveOperations() == target->second->getNAddRemoveOperations() && 
                repairSet->getNFlipEdgesOperations() == target->second->getNFlipEdgesOperations() &&
                repairSet->getNRepairOperations() > target->second->getNRepairOperations())
            {
                return;
            }
            
            //update values for better solutions
            if(repairSet->getNRepairOperations() < target->second->getNRepairOperations())
            {
                nTopologyChanges_ -= target->second->getNTopologyChanges();
                nTopologyChanges_ += repairSet->getNTopologyChanges();
                nAROperations -= target->second->getNAddRemoveOperations();
                nAROperations += repairSet->getNAddRemoveOperations();
                nEOperations -= target->second->getNFlipEdgesOperations();
                nEOperations += repairSet->getNFlipEdgesOperations();
                nRepairOperations_ -= target->second->getNRepairOperations();
                nRepairOperations_ += repairSet->getNRepairOperations();
            }
        }
        target->second->addRepairSet(repairSet);
    }
}


int InconsistencySolution::getNTopologyChanges() {
    return nTopologyChanges_;
}

int InconsistencySolution::getNRepairOperations() {
    return nRepairOperations_;
}

void InconsistencySolution::printSolution(int verboseLevel, bool printAll) {
    if(verboseLevel < 2)
    {
        return printParsableSolution(verboseLevel);
    }
    if(verboseLevel == 3)
    {
        return printJSONSolution(printAll);
    }
    std::cout << "### Found solution with " << nRepairOperations_ << " repair operations." << std::endl;
    for(auto iNode = iNodes_.begin(), iNodesEnd = iNodes_.end(); iNode != iNodesEnd; iNode++)
    {
        std::cout << "\tInconsistent node " << iNode->second->id_ << "." << std::endl;
        int i = 1;
        for(auto repair = iNode->second->repairSet_.begin(), repairEnd = iNode->second->repairSet_.end(); repair!=repairEnd;repair++)
        {
            if(printAll)
            {
                std::cout << "\t\tRepair #" << i << ":" << std::endl;
                i++;
            }
            for(auto it = (*repair)->repairedFunctions_.begin(), end = (*repair)->repairedFunctions_.end(); it != end; it++)
            {
                std::cout << "\t\t\tChange function of " << (*it)->getNode() << " to " << (*it)->printFunction() << std::endl;
            }
            for(auto it = (*repair)->flippedEdges_.begin(), end = (*repair)->flippedEdges_.end(); it != end; it++)
            {
                std::cout << "\t\t\tFlip sign of edge (" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ")." << std::endl;
            }
            for(auto it = (*repair)->removedEdges_.begin(), end = (*repair)->removedEdges_.end(); it != end; it++)
            {
                std::cout << "\t\t\tRemove edge (" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ")." << std::endl;
            }
            for(auto it = (*repair)->addedEdges_.begin(), end = (*repair)->addedEdges_.end(); it != end; it++)
            {
                std::cout << "\t\t\tAdd edge (" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ") with sign " << (*it)->getSign() << "." << std::endl;
            }
            if(!printAll)
            {
                break;
            }
        }
    }
    if(Configuration::isActive("labelling"))
    {
        std::cout << "\t### Labelling for this solution:" << std::endl;
        bool multipleProfiles = Configuration::isActive("multipleProfiles");
        for(auto it = vlabel_.begin(), end = vlabel_.end(); it != end; it++)
        {
            if(multipleProfiles)
                std::cout << "\t\tProfile: " << it->first << std::endl;
            for(auto it2 = it->second.begin(), end2 = it->second.end(); it2 != end2; it2++)
            {
                std::cout << "\t\t\tTime step: " << it2->first << std::endl;
                for(auto it3 = it2->second.begin(), end3 = it2->second.end(); it3 != end3; it3++)
                {
                    std::cout << "\t\t\t\t" << it3->first << " => " << it3->second << std::endl;
                }
            }
        }
    }
}

void InconsistencySolution::printParsableSolution(int verboseLevel) {
    if(verboseLevel > 0)
        std::cout << "[";
    for(auto iNode = iNodes_.begin(), iNodesEnd = iNodes_.end(); iNode != iNodesEnd; iNode++)
    {
        if(iNode != iNodes_.begin())
        {
            std::cout << (verboseLevel > 0 ? ";" : "/");
        }
        std::cout << iNode->second->id_ << (verboseLevel > 0 ? ":{" : "@");

        for(auto repair = iNode->second->repairSet_.begin(), repairEnd = iNode->second->repairSet_.end(); repair!=repairEnd;repair++)
        {
            if(repair != iNode->second->repairSet_.begin())
            {
                std::cout << ";";
            }
            if(verboseLevel > 0)
                std::cout << "{";
            
            bool first = true;

            for(auto it = (*repair)->addedEdges_.begin(), end = (*repair)->addedEdges_.end(); it != end; it++)
            {
                if(!first)
                {
                    std::cout << (verboseLevel > 0 ? ";" : ":");
                }
                first = false;
                if(verboseLevel > 0)
                {
                    std::cout << "A:(" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << "," << (*it)->getSign() << ")";
                }
                else
                {
                    std::cout << "A," << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << "," << (*it)->getSign();
                }
            }
            for(auto it = (*repair)->removedEdges_.begin(), end = (*repair)->removedEdges_.end(); it != end; it++)
            {
                if(!first)
                {
                    std::cout << (verboseLevel > 0 ? ";" : ":");
                }
                first = false;
                if(verboseLevel > 0)
                {
                    std::cout << "R:(" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ")";
                }
                else
                {
                    std::cout << "R," << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_;
                }
            }
            for(auto it = (*repair)->flippedEdges_.begin(), end = (*repair)->flippedEdges_.end(); it != end; it++)
            {
                if(!first)
                {
                    std::cout << (verboseLevel > 0 ? ";" : ":");
                }
                first = false;
                if(verboseLevel > 0)
                {
                    std::cout << "E:(" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ")";
                }
                else
                {
                    std::cout << "E," << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_;
                }
            }
            for(auto it = (*repair)->repairedFunctions_.begin(), end = (*repair)->repairedFunctions_.end(); it != end; it++)
            {
                if(!first)
                {
                    std::cout << (verboseLevel > 0 ? ";" : ":");
                }
                first = false;
                std::cout << "F" << (verboseLevel > 0 ? ":" : ",") << (*it)->printFunction();
            }
            if(verboseLevel > 0)
                std::cout << "}";

        }
        if(verboseLevel > 0)
            std::cout << "}";
    }
    if(verboseLevel > 0)
        std::cout << "]";
    std::cout << std::endl;
}


void InconsistencySolution::printJSONSolution(bool printAll) {
    std::cout << "{" << std::endl;
    std::cout << "\t\"solution_repairs\": " << nRepairOperations_ << "," << std::endl;
    std::cout << "\t\"node_repairs\": [" << std::endl;

    bool firstNode = true;
    for(auto iNode = iNodes_.begin(), iNodesEnd = iNodes_.end(); iNode != iNodesEnd; iNode++)
    {
        if(firstNode)
        {
            firstNode = false;
        }
        else
        {
            std::cout << "," << std::endl;
        }
        std::cout << "\t\t{" << std::endl;
        //std::cout << "\t\t\t\"node\": " << Util_h::getOutputName(iNode->second->id_) << "," << std::endl;
        std::cout << "\t\t\t\"node\": \"" << Util_h::cleanString(iNode->second->id_) << "\"," << std::endl;
        std::cout << "\t\t\t\"repair_set\": [" << std::endl;
        bool firstRepairSet = true;
        int i = 1;
        for(auto repair = iNode->second->repairSet_.begin(), repairEnd = iNode->second->repairSet_.end(); repair!=repairEnd;repair++)
        {
            if(firstRepairSet)
            {
                firstRepairSet = false;
            }
            else
            {
                std::cout << "," << std::endl;
            }
            std::cout << "\t\t\t\t{" << std::endl;
            std::cout << "\t\t\t\t\t\"repair_id\": " << i << std::endl;
            i++;
            std::cout << "\t\t\t\t\t\"repairs\": [" << std::endl;
            bool firstRepair = true;
            for(auto it = (*repair)->repairedFunctions_.begin(), end = (*repair)->repairedFunctions_.end(); it != end; it++)
            {
                if(firstRepair)
                {
                    firstRepair = false;
                }
                else
                {
                    std::cout << "," << std::endl;
                }
                std::cout << "\t\t\t\t\t\t{" << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"type\": \"F\"," << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"value\": \"" << Util_h::cleanString((*it)->printFunction()) << "\"" << std::endl;
                std::cout << "\t\t\t\t\t\t}";
            }
            for(auto it = (*repair)->flippedEdges_.begin(), end = (*repair)->flippedEdges_.end(); it != end; it++)
            {
                if(firstRepair)
                {
                    firstRepair = false;
                }
                else
                {
                    std::cout << "," << std::endl;
                }
                std::cout << "\t\t\t\t\t\t{" << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"type\": \"E\"," << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"value\": \"(" << Util_h::cleanString((*it)->getStart()->id_) << "," << Util_h::cleanString((*it)->getEnd()->id_) << ")\"" << std::endl;
                std::cout << "\t\t\t\t\t\t}";
            }
            for(auto it = (*repair)->removedEdges_.begin(), end = (*repair)->removedEdges_.end(); it != end; it++)
            {
                if(firstRepair)
                {
                    firstRepair = false;
                }
                else
                {
                    std::cout << "," << std::endl;
                }
                std::cout << "\t\t\t\t\t\t{" << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"type\": \"R\"," << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"value\": \"(" << Util_h::cleanString((*it)->getStart()->id_) << "," << Util_h::cleanString((*it)->getEnd()->id_) << ")\"" << std::endl;
                std::cout << "\t\t\t\t\t\t}";
            }
            for(auto it = (*repair)->addedEdges_.begin(), end = (*repair)->addedEdges_.end(); it != end; it++)
            {
                if(firstRepair)
                {
                    firstRepair = false;
                }
                else
                {
                    std::cout << "," << std::endl;
                }
                std::cout << "\t\t\t\t\t\t{" << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"type\": \"A\"," << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"value\": \"(" << Util_h::cleanString((*it)->getStart()->id_) << "," << Util_h::cleanString((*it)->getEnd()->id_) << ")\"," << std::endl;
                std::cout << "\t\t\t\t\t\t\t\"sign\": " << (*it)->getSign() << std::endl;
                std::cout << "\t\t\t\t\t\t}";
            }
            std::cout << std::endl;
            std::cout << "\t\t\t\t\t]" << std::endl;
            std::cout << "\t\t\t\t}";
            if(!printAll)
            {
                break;
            }
        }
        std::cout << std::endl;
        std::cout << "\t\t\t]" << std::endl;
        std::cout << "\t\t}";
    }
    std::cout << std::endl;
    std::cout << "\t]" << std::endl;
    std::cout << "}" << std::endl;
}

void InconsistencySolution::printInconsistency(std::string prefix) {
    std::cout << prefix << "\"nodes\": [";
    bool first = true;
    for(auto iNode = iNodes_.begin(), iNodesEnd = iNodes_.end(); iNode != iNodesEnd; iNode++)
    {
        if(first)
        {
            first = false;
        }
        else
        {
            std::cout << ",";
        }
        std::cout << "\"" << Util_h::cleanString(iNode->second->id_) << "\"";
    }
    std::cout << "]," << std::endl;
    std::cout << prefix << "\"profiles\": [";
    first = true;
    for(auto iProfile = iProfiles_.begin(), iProfileEnd = iProfiles_.end(); iProfile != iProfileEnd; iProfile++)
    {
        if(first)
        {
            first = false;
        }
        else
        {
            std::cout << ",";
        }
        std::cout << "\"" << Util_h::cleanString(iProfile->first) << "\"";
    }
    std::cout << "]" << std::endl;
}

/*
* returns -1 if argument is better solution
* returns 0 if argument is equal
* returns 1 if this is better solution
*/
int InconsistencySolution::compareRepairs(InconsistencySolution* solution)
{

    if(nAROperations < solution->nAROperations)
        return 1;
    if(nAROperations > solution->nAROperations)
        return -1;
    
    if(nEOperations < solution->nEOperations)
        return 1;
    if(nEOperations > solution->nEOperations)
        return -1;

    if(nRepairOperations_ < solution->nRepairOperations_)
        return 1;
    if(nRepairOperations_ > solution->nRepairOperations_)
        return -1;
    return 0;
}


InconsistentNode* InconsistencySolution::getINode(std::string id)
{
    auto target = iNodes_.find(id);
    if(target != iNodes_.end())
    {
        return target->second;
    }
    return nullptr;
}



InconsistentNode::InconsistentNode(std::string id, bool generalization)
    :id_(id),
    generalization_(generalization),
    repairSet_(){
        nTopologyChanges_ = 0;
        nRepairOperations_ = 0;
        nAddRemoveOperations_ = 0;
        nFlipEdgesOperations_ = 0;
        repaired_ = false;
        topologicalError_ = false;
        if(generalization)
        {
            repairType = 1;
        }
        else
        {
            repairType = 2;
        }
        
    }

InconsistentNode::~InconsistentNode(){}


void InconsistentNode::addRepairSet(RepairSet* repairSet)
{
    if(!repaired_)
    {
        repaired_ = true;
        nTopologyChanges_ = repairSet->getNTopologyChanges();
        nRepairOperations_= repairSet->getNRepairOperations();
        nFlipEdgesOperations_ = repairSet->getNFlipEdgesOperations();
        nAddRemoveOperations_ = repairSet->getNAddRemoveOperations();
    }
    else
    {
        //if new solutions is worse, ignore
        if(repairSet->getNAddRemoveOperations() > nAddRemoveOperations_ )
        {
            return;
        }
        if(repairSet->getNAddRemoveOperations() == nAddRemoveOperations_ && 
            repairSet->getNFlipEdgesOperations() > nFlipEdgesOperations_)
        {
            return;
        }
        if(repairSet->getNAddRemoveOperations() == nAddRemoveOperations_ && 
            repairSet->getNFlipEdgesOperations() == nFlipEdgesOperations_ &&
            repairSet->getNRepairOperations() > nRepairOperations_)
        {
            return;
        }

        //if new solution is better, remove all others before
        //at this point we know that the new repair is at least as good as existing ones
        if(repairSet->getNRepairOperations() < nRepairOperations_)
        {
            repairSet_.erase(repairSet_.begin(), repairSet_.end());
            repairSet_.clear();
            nTopologyChanges_ = repairSet->getNTopologyChanges();
            nRepairOperations_= repairSet->getNRepairOperations();
            nAddRemoveOperations_ = repairSet->getNAddRemoveOperations();
            nFlipEdgesOperations_ = repairSet->getNFlipEdgesOperations();
        }
        //if(repairSet->getNTopologyChanges() < nTopologyChanges_ ||
        //    repairSet->getNRepairOperations() < nRepairOperations_)
        //{
        //    repairSet_.clear();
        //    nTopologyChanges_ = repairSet->getNTopologyChanges();
        //    nRepairOperations_= repairSet->getNRepairOperations();
        //}
    }
    repairSet_.push_back(repairSet);
}

int InconsistentNode::getNTopologyChanges() {
    return nTopologyChanges_;
}

int InconsistentNode::getNRepairOperations() {
    return nRepairOperations_;
}

int InconsistentNode::getNAddRemoveOperations() {
    return nAddRemoveOperations_;
}

int InconsistentNode::getNFlipEdgesOperations() {
    return nFlipEdgesOperations_;
}


RepairSet::RepairSet()
    :repairedFunctions_(),
    flippedEdges_(),
    removedEdges_(),
    addedEdges_() {
        nTopologyChanges_ = 0;
        nRepairOperations_ = 0;
        nAddRemoveOperations_ = 0;
        nFlipEdgesOperations_ = 0;
    }

RepairSet::~RepairSet(){
    repairedFunctions_.erase(repairedFunctions_.begin(), repairedFunctions_.end());
    repairedFunctions_.clear();
    flippedEdges_.clear();
    removedEdges_.clear();
    addedEdges_.erase(addedEdges_.begin(), addedEdges_.end());
    addedEdges_.clear();
}


int RepairSet::getNTopologyChanges() {
    return nTopologyChanges_;
}

int RepairSet::getNRepairOperations() {
    return nRepairOperations_;
}

int RepairSet::getNFlipEdgesOperations() {
    return nFlipEdgesOperations_;
}

int RepairSet::getNAddRemoveOperations() {
    return nAddRemoveOperations_;
}

void RepairSet::addRepairedFunction(Function* f) {
    repairedFunctions_.push_back(f);
    nRepairOperations_++;
}


void RepairSet::addFlippedEdge(Edge* e) {
    flippedEdges_.push_back(e);
    nRepairOperations_++;
    nTopologyChanges_++;
    nFlipEdgesOperations_++;
}

void RepairSet::removeEdge(Edge* e) {
    removedEdges_.push_back(e);
    nRepairOperations_++;
    nTopologyChanges_++;
    nAddRemoveOperations_++;
}

void RepairSet::addEdge(Edge* e) {
    addedEdges_.push_back(e);
    nRepairOperations_++;
    nTopologyChanges_++;
    nAddRemoveOperations_++;
}

bool RepairSet::isEqual(RepairSet* repairSet)
{

    if(nTopologyChanges_ != repairSet->nTopologyChanges_ ||
        nRepairOperations_ != repairSet->nRepairOperations_ ||
        nFlipEdgesOperations_ != repairSet->nFlipEdgesOperations_ ||
        nAddRemoveOperations_ != repairSet->nAddRemoveOperations_ ||
        repairedFunctions_.size() != repairSet->repairedFunctions_.size() ||
        flippedEdges_.size() != repairSet->flippedEdges_.size() ||
        removedEdges_.size() != repairSet->removedEdges_.size() ||
        addedEdges_.size() != repairSet->addedEdges_.size())
    {
        return false;
    }

    //check if functions are the same
    for(auto it = repairedFunctions_.begin(), end = repairedFunctions_.end(); it != end; it++)
    {
        bool found = false;
        for(auto it2 = repairSet->repairedFunctions_.begin(), end2 = repairSet->repairedFunctions_.end(); it2 != end2; it2++)
        {
            if((*it)->isEqual((*it2)))
            {
                found = true;
                break;
            }
        }
        if(!found)
            return false;
    }

    //check if flip edge sign operations are the same
    for(auto it = flippedEdges_.begin(), end = flippedEdges_.end(); it != end; it++)
    {
        bool found = false;
        for(auto it2 = repairSet->flippedEdges_.begin(), end2 = repairSet->flippedEdges_.end(); it2 != end2; it2++)
        {
            if((*it)->isEqual((*it2)))
            {
                found = true;
                break;
            }
        }
        if(!found)
            return false;
    }

    //check if removed edges are the same
    for(auto it = removedEdges_.begin(), end = removedEdges_.end(); it != end; it++)
    {
        bool found = false;
        for(auto it2 = repairSet->removedEdges_.begin(), end2 = repairSet->removedEdges_.end(); it2 != end2; it2++)
        {
            if((*it)->isEqual((*it2)))
            {
                found = true;
                break;
            }
        }
        if(!found)
            return false;
    }

    //check if added edges are the same
    for(auto it = addedEdges_.begin(), end = addedEdges_.end(); it != end; it++)
    {
        bool found = false;
        for(auto it2 = repairSet->addedEdges_.begin(), end2 = repairSet->addedEdges_.end(); it2 != end2; it2++)
        {
            if((*it)->isEqual((*it2)))
            {
                found = true;
                break;
            }
        }
        if(!found)
            return false;
    }



    return true;
}