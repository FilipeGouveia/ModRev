#include "Network.h"

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "Configuration.h"

Network::Network() 
    :nodes_(),
    edges_(){}

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
        regFunction_ = nullptr;
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


Function::Function(std::string node, int nClauses)
    :node_(node),
    nClauses_(nClauses)
{
    for(int i = 1; i <= nClauses; i++)
    {
        std::vector<std::string> clause;
        clauses_.insert(std::make_pair(i,clause));
    }
    regulatorsMap_ = std::map<std::string,int>();
    level_ = 0;

}

Function::~Function() {}

void Function::addElementClause(int id, std::string node) {
    if(id > nClauses_)
    {
        for(int i = nClauses_ + 1; i <= id; i++)
        {
            std::vector<std::string> clause;
            clauses_.insert(std::make_pair(i,clause));
        }
        nClauses_ = id;
    }
    if(id > 0 && id <= nClauses_)
    {
        auto it = clauses_.find(id);
        if(it != clauses_.end())
        {
            it->second.push_back(node);
        }
    }
}


int Function::getNumberOfRegulators(){
    return getRegulatorsMap().size();
}


std::map<std::string,int> Function::getRegulatorsMap(){
    if(regulatorsMap_.empty())
    {
        int index = 1;
        std::map<std::string,int> elements;
        for(int i = 1; i <= nClauses_; i++)
        {
            std::vector<std::string> clause = clauses_.find(i)->second;
            for(auto it = clause.begin(), end=clause.end(); it!=end; it++)
            {
                auto ret = elements.insert(std::make_pair((*it),index));
                if(ret.second != false)
                    index++;
            }
        }
        regulatorsMap_ = elements;
    }
    return regulatorsMap_;
}

std::string Function::printFunction(){
    std::string result = "";
    for(int i = 1; i <= nClauses_; i++)
    {
        result += "(";
        std::vector<std::string> clause = clauses_[i];
        bool first = true;
        for(auto it = clause.begin(), end = clause.end(); it!=end; it++)
        {
            if(!first)
            {
                result += " && ";
            }
            first = false;
            result += (*it);
        }
        result += ")";
        if(i < nClauses_)
        {
            result += " || ";
        }
    }
    return result;
}

bool Function::isEqual(Function* f)
{
    if(nClauses_ != f->nClauses_ || node_.compare(f->node_) != 0)
        return false;

    for(int i = 1; i <= nClauses_; i++)
    {
        //to prevent some function having duplicated clauses, we double check inclusiveness
        if(!Function::isClausePresent(clauses_[i], f->clauses_))
            return false;
        if(!Function::isClausePresent(f->clauses_[i], clauses_))
            return false;
    }
    
    return true;
}

bool Function::isClausePresent(std::vector<std::string> clause, std::map<int, std::vector<std::string>> clauses)
{

    for(auto it = clauses.begin(), end = clauses.end(); it!=end; it++)
    {
        std::vector<std::string> possMatch = it->second;
        bool isPossibleMatch = true;

        for(auto itElem = clause.begin(), endElem = clause.end(); itElem != endElem; itElem++)
        {
            bool elemFound = false;

            for(auto itPossElem = possMatch.begin(), endPossElem = possMatch.end(); itPossElem != endPossElem; itPossElem++)
            {
                if((*itElem).compare((*itPossElem)) == 0)
                {
                    elemFound = true;
                    break;
                }
            }

            if(!elemFound)
            {
                isPossibleMatch = false;
                break;
            }

        }
        
        if(isPossibleMatch)
        {
            bool isTrueMatch = true;
            //double check to prevent duplicated values originating false positives
            for(auto itElem = possMatch.begin(), endElem = possMatch.end(); itElem != endElem; itElem++)
            {
                bool elemFound = false;

                for(auto itPossElem = clause.begin(), endPossElem = clause.end(); itPossElem != endPossElem; itPossElem++)
                {
                    if((*itElem).compare((*itPossElem)) == 0)
                    {
                        elemFound = true;
                        break;
                    }
                }

                if(!elemFound)
                {
                    isTrueMatch = false;
                    break;
                }

            }
            if(isTrueMatch)
                return true;
        }

    }
    return false;
}


InconsistencySolution::InconsistencySolution()
    :iNodes_(),
    vlabel_(){
        nTopologyChanges_ = 0;
        nRepairOperations_ = 0;
        hasImpossibility = false;
    }

InconsistencySolution::~InconsistencySolution(){}

void InconsistencySolution::addGeneralization(std::string id) {

    InconsistentNode* newINode = new InconsistentNode(id, true);
    iNodes_.insert(std::make_pair(id, newINode));

}


void InconsistencySolution::addParticularization(std::string id) {

    InconsistentNode* newINode = new InconsistentNode(id, false);
    iNodes_.insert(std::make_pair(id, newINode));

}


void InconsistencySolution::addVLabel(std::string profile, std::string id, int value) {
    
    if(vlabel_.find(profile) == vlabel_.end())
    {
        std::map<std::string, int> newMap;
        vlabel_.insert(std::make_pair(profile, newMap));
    }
    vlabel_.find(profile)->second.insert(std::make_pair(id, value));

}

void InconsistencySolution::addRepairSet(std::string id, RepairSet* repairSet)
{
    auto target = iNodes_.find(id);
    if(target != iNodes_.end())
    {
        if(!target->second->repaired_)
        {
            nTopologyChanges_ += repairSet->getNTopologyChanges();
            nRepairOperations_ += repairSet->getNRepairOperations();
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

void InconsistencySolution::printSolution(bool printAll) {
    std::cout << "### Found solution with " << nRepairOperations_ << " repair operations." << std::endl;
    for(auto iNode = iNodes_.begin(), iNodesEnd = iNodes_.end(); iNode != iNodesEnd; iNode++)
    {
        for(auto repair = iNode->second->repairSet_.begin(), repairEnd = iNode->second->repairSet_.end(); repair!=repairEnd;repair++)
        {
            for(auto it = (*repair)->repairedFunctions_.begin(), end = (*repair)->repairedFunctions_.end(); it != end; it++)
            {
                std::cout << "\tChange function of " << (*it)->node_ << " to " << (*it)->printFunction() << std::endl;
            }
            for(auto it = (*repair)->flippedEdges_.begin(), end = (*repair)->flippedEdges_.end(); it != end; it++)
            {
                std::cout << "\tFlip sign of edge (" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ")." << std::endl;
            }
            for(auto it = (*repair)->removedEdges_.begin(), end = (*repair)->removedEdges_.end(); it != end; it++)
            {
                std::cout << "\tRemove edge (" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ")." << std::endl;
            }
            for(auto it = (*repair)->addedEdges_.begin(), end = (*repair)->addedEdges_.end(); it != end; it++)
            {
                std::cout << "\tAdd edge (" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ") with sign " << (*it)->getSign() << "." << std::endl;
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
                std::cout << "\t\t" << it2->first << " => " << it2->second << std::endl;
            }
        }
    }
}



InconsistentNode::InconsistentNode(std::string id, bool generalization)
    :id_(id),
    generalization_(generalization),
    repairSet_(){
        nTopologyChanges_ = 0;
        nRepairOperations_ = 0;
        repaired_ = false;
    }

InconsistentNode::~InconsistentNode(){}


void InconsistentNode::addRepairSet(RepairSet* repairSet)
{
    repairSet_.push_back(repairSet);
    if(!repaired_)
    {
        repaired_ = true;
        nTopologyChanges_ = repairSet->getNTopologyChanges();
        nRepairOperations_= repairSet->getNRepairOperations();

    }

}

void InconsistentNode::printSolution(bool printAll)
{
    //TODO
    return;
}

int InconsistentNode::getNTopologyChanges() {
    return nTopologyChanges_;
}

int InconsistentNode::getNRepairOperations() {
    return nRepairOperations_;
}


RepairSet::RepairSet()
    :repairedFunctions_(),
    flippedEdges_(),
    removedEdges_(),
    addedEdges_() {
        nTopologyChanges_ = 0;
        nRepairOperations_ = 0;
    }

RepairSet::~RepairSet(){}


int RepairSet::getNTopologyChanges() {
    return nTopologyChanges_;
}

int RepairSet::getNRepairOperations() {
    return nRepairOperations_;
}


void RepairSet::addRepairedFunction(Function* f) {
    repairedFunctions_.push_back(f);
    nRepairOperations_++;
}


void RepairSet::addFlippedEdge(Edge* e) {
    flippedEdges_.push_back(e);
    nRepairOperations_++;
    nTopologyChanges_++;
}

void RepairSet::removeEdge(Edge* e) {
    removedEdges_.push_back(e);
    nRepairOperations_++;
    nTopologyChanges_++;
}

void RepairSet::addEdge(Edge* e) {
    addedEdges_.push_back(e);
    nRepairOperations_++;
    nTopologyChanges_++;
}

bool RepairSet::isEqual(RepairSet* repairSet)
{

    if(nTopologyChanges_ != repairSet->nTopologyChanges_ ||
        nRepairOperations_ != repairSet->nRepairOperations_ ||
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