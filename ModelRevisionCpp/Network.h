#ifndef NETWORK_H_
#define NETWORK_H_

#include <map>
#include <vector>
#include <string>

class Function {

    private:
        std::map<std::string,int> regulatorsMap_;

    public:

        std::string node_;
        int nClauses_;
        std::map<int, std::vector< std::string >> clauses_;

        Function(std::string node, int nClauses);
        ~Function();

        void addElementClause(int id, std::string node);
        int getNumberOfRegulators();
        std::map<std::string,int> getRegulatorsMap();
        std::string printFunction();
    
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
        void flipSign();

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
        Edge* getEdge(std::string start, std::string end);
        Edge* addEdge(Node* start, Node* end, int sign);

};


class Solution {

    public:
        std::vector<std::string> generalization_;
        std::vector<std::string> particularization_;
        std::map<std::string,int> vlabel_;

        int nTopologyChanges_;
        int nRepairOperations_;
        std::vector<Function*> repairedFunctions_;
        std::vector<Edge*> flippedEdges_;
        bool hasImpossibility;

        Solution();

        void addGeneralization(std::string id);
        void addParticularization(std::string id);
        void addVLabel(std::string id, int value);

        int getNTopologyChanges();
        void addRepairedFunction(Function* f);
        void addFlippedEdge(Edge* e);
        void printSolution();
};

#endif
