#ifndef NETWORK_H_
#define NETWORK_H_

#include <map>
#include <vector>
#include <string>

class Function {

    public:

        std::string node_;
        int nClauses_;
        std::map<int, std::vector< std::string >> clauses_;

        Function(std::string node, int nClauses);
        ~Function();

        void addElementClause(int id, std::string node);


};


class Node {

    public:

        std::string id_;
        Function* regFunction_;

        Node(std::string id);
        ~Node();

        Function* addFunction(Function* regulation);
        Function* getFunction();

};

class Edge {

    public:

        Node* start_;
        Node* end_;
        int sign_;

        Edge(Node* start, Node* end, int sign);
        ~Edge();

        Node* getStart();
        Node* getEnd();
        int getSign();

};



class Network {

    public:
        
        std::map< std::string, Node* > nodes_;
        std::vector< Edge* > edges_;

        Network();
        ~Network();

        std::vector< Node* > getNodes();
        Node* addNode(std::string id);
        Node* getNode(std::string id);

        std::vector< Edge* > getEdges();
        Edge* addEdge(Node* start, Node* end, int sign);

};


class FunctionRepairs {

    public:
        std::vector<std::string> generalization_;
        std::vector<std::string> particularization_;
        std::map<std::string,int> vlabel_;

        FunctionRepairs();

        void addGeneralization(std::string id);
        void addParticularization(std::string id);
        void addVLabel(std::string id, int value);
};

#endif
