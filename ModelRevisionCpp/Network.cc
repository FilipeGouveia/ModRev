#include "Network.h"

#include <string>
#include <map>
#include <vector>

Network::Network() 
    :nodes_(),
    edges_(){};

Network::~Network() {

    for(auto it = nodes_.begin(), end = nodes_.end(); it != end; it++)
    {
        delete it->second;
    }

    for(auto it = edges_.begin(), end = edges_.end(); it != end; it++)
    {
        delete (*it);
    }
};

std::vector< Node* > Network::getNodes() {

    std::vector< Node* > result;
    for(auto it = nodes_.begin(), end = nodes_.end(); it != end; it++)
    {
        result.push_back(it->second);
    }
    return result;

};

Node* Network::addNode(std::string id) {
    Node* node = new Node(id);
    auto ret = nodes_.insert(std::make_pair(id, node));
    if(ret.second == false)
    {
        delete node;
        return ret.first->second;
    }
    return node;
};

Node* Network::getNode(std::string id) {
    auto it = nodes_.find(id);
    if(it != nodes_.end())
        return it->second;
    return nullptr;
};

std::vector< Edge* > Network::getEdges() {
    return edges_;
};

Edge* Network::addEdge(Node* start, Node* end, int sign) {
    Edge* edge = new Edge(start, end, sign);
    edges_.push_back(edge);
    return edge;
};

Edge::Edge(Node* start, Node* end, int sign)
    :start_(start),
    end_(end),
    sign_(sign){};

Edge::~Edge() {};

Node* Edge::getStart() {
    return start_;
};

Node* Edge::getEnd() {
    return end_;
};

int Edge::getSign() {
    return sign_;
};


Node::Node(std::string id)
    :id_(id)
    {
        regFunction_ = nullptr;
    };

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
};


Function::Function(std::string node, int nClauses)
    :node_(node),
    nClauses_(nClauses)
{
    for(int i = 1; i <= nClauses; i++)
    {
        std::vector<std::string> clause;
        clauses_.insert(std::make_pair(i,clause));
    }
};

Function::~Function() {};

void Function::addElementClause(int id, std::string node) {
    if(id > 0 && id <= nClauses_)
    {
        auto it = clauses_.find(id);
        if(it != clauses_.end())
        {
            it->second.push_back(node);
        }
    }
};


int Function::getNumberOfRegulators(){
    return getRegulatorsMap().size();
};

std::map<std::string,int> Function::getRegulatorsMap(){
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
    return elements;
};



FunctionRepairs::FunctionRepairs()
    :generalization_(),
    particularization_(),
    vlabel_() {};

void FunctionRepairs::addGeneralization(std::string id) {

    for(auto it = generalization_.begin(), end = generalization_.end(); it != end; it++)
    {
        if((*it) == id)
            return;
    }
    generalization_.push_back(id);

};


void FunctionRepairs::addParticularization(std::string id) {

    for(auto it = particularization_.begin(), end = particularization_.end(); it != end; it++)
    {
        if((*it) == id)
            return;
    }
    particularization_.push_back(id);

};


void FunctionRepairs::addVLabel(std::string id, int value) {

    vlabel_.insert(std::make_pair(id, value));

};
