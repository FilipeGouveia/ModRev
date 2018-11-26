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


Solution::Solution()
    :generalization_(),
    particularization_(),
    vlabel_(),
    repairedFunctions_(),
    flippedEdges_() {
        nTopologyChanges_ = 0;
        nRepairOperations_ = 0;
        hasImpossibility = false;
    }

void Solution::addGeneralization(std::string id) {

    for(auto it = generalization_.begin(), end = generalization_.end(); it != end; it++)
    {
        if((*it) == id)
            return;
    }
    generalization_.push_back(id);

}


void Solution::addParticularization(std::string id) {

    for(auto it = particularization_.begin(), end = particularization_.end(); it != end; it++)
    {
        if((*it) == id)
            return;
    }
    particularization_.push_back(id);

}


void Solution::addVLabel(std::string profile, std::string id, int value) {
    
    if(vlabel_.find(profile) == vlabel_.end())
    {
        std::map<std::string, int> newMap;
        vlabel_.insert(std::make_pair(profile, newMap));
    }
    vlabel_.find(profile)->second.insert(std::make_pair(id, value));

}


int Solution::getNTopologyChanges() {
    return nTopologyChanges_;
}


void Solution::addRepairedFunction(Function* f) {
    repairedFunctions_.push_back(f);
    nRepairOperations_++;
}


void Solution::addFlippedEdge(Edge* e) {
    flippedEdges_.push_back(e);
    nRepairOperations_++;
    nTopologyChanges_++;
}


void Solution::printSolution() {
    std::cout << "### Found solution with " << nRepairOperations_ << " repair operations." << std::endl;
    for(auto it = flippedEdges_.begin(), end = flippedEdges_.end(); it != end; it++)
    {
        std::cout << "\tFlip sign of edge (" << (*it)->getStart()->id_ << "," << (*it)->getEnd()->id_ << ")." << std::endl;
    }
    for(auto it = repairedFunctions_.begin(), end = repairedFunctions_.end(); it != end; it++)
    {
        std::cout << "\tChange function of " << (*it)->node_ << " to " << (*it)->printFunction() << std::endl;
    }
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