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
        int level_;
        bool son_consistent;

        Function(std::string node, int nClauses);
        ~Function();

        void addElementClause(int id, std::string node);
        int getNumberOfRegulators();
        std::map<std::string,int> getRegulatorsMap();
        std::string printFunction();
        bool isEqual(Function* f);

        static bool isClausePresent(std::vector<std::string> clause, std::map<int, std::vector<std::string>> clauses);
    
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
        bool fixed_;

        Edge(Node* start, Node* end, int sign);
        ~Edge();

        Node* getStart();
        Node* getEnd();
        int getSign();
        void flipSign();
        bool isFixed();
        void setFixed();
        bool isEqual(Edge* e, bool checkSign = false);

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
        void removeEdge(std::string start, std::string end);
        void removeEdge(Edge * e);
        void addEdge(Edge * e);

};


class RepairSet {

    public:
        int nTopologyChanges_;
        int nRepairOperations_;
        std::vector<Function*> repairedFunctions_; //should only be one per repairSet per inconsistent node
        std::vector<Edge*> flippedEdges_;
        std::vector<Edge*> removedEdges_;
        std::vector<Edge*> addedEdges_;

        RepairSet();
        ~RepairSet();

        int getNTopologyChanges();
        int getNRepairOperations();
        void addRepairedFunction(Function* f);
        void addFlippedEdge(Edge* e);
        void removeEdge(Edge* e);
        void addEdge(Edge* e);
        bool isEqual(RepairSet* repairSet);
};


class InconsistentNode {

    public:
        std::string id_;
        bool generalization_;
        int nTopologyChanges_;
        int nRepairOperations_;
        bool repaired_;
        //possible ways to repair the node;
        std::vector<RepairSet*> repairSet_;

        //debug repair type: 0 - no info; 1 - gen; 2 - part; 3 - both
        int repairType;

        InconsistentNode(std::string id, bool generalization);
        ~InconsistentNode();

        int getNTopologyChanges();
        int getNRepairOperations();
        void addRepairSet(RepairSet* repairSet);
};


class InconsistencySolution {

    public:
        //map node id as key
        std::map<std::string, InconsistentNode*> iNodes_;
        //map profile as key; second map node as key;
        std::map<std::string, std::map<std::string,int>> vlabel_;
        int nTopologyChanges_;
        int nRepairOperations_;
        bool hasImpossibility;

        InconsistencySolution();
        ~InconsistencySolution();

        void addGeneralization(std::string id);
        void addParticularization(std::string id);
        void addVLabel(std::string profile, std::string id, int value);
        void addRepairSet(std::string id, RepairSet* repairSet);
        int getNTopologyChanges();
        int getNRepairOperations();
        void printSolution(bool printAll = true);

};
#endif
