#ifndef NETWORK_H_
#define NETWORK_H_

#include <map>
#include <vector>
#include <string>
#include <BooleanFunction.h>
#include <set>

class Function {

    private:
        BooleanFunction::Function * booleanF_;

    public:

        int distanceFromOriginal_;
        bool son_consistent;

        Function(BooleanFunction::Function *f);
        Function(std::string node);
        ~Function();

        std::string getNode();
        void addElementClause(int id, std::string node);
        int getNumberOfRegulators();
        std::map<std::string,int> getRegulatorsMap();

        std::map<int, std::set<std::string>> getClauses();
        int getNClauses();

        std::string printFunction();
        bool isEqual(Function* f);

        std::vector<int> getFullLevel();
        int compareLevel(Function * f);
        int compareLevel(std::vector<int> fullLevel);
        std::string printFunctionFullLevel();

        std::vector<Function*> getParents();
        std::vector<Function*> getChildren();

        std::vector<Function*> getReplacements(bool generalize);

        BooleanFunction::Function * getBooleanFunction();

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
        std::string input_file_network_;
        std::vector< std::string> observation_files;

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
        int nAddRemoveOperations_;
        int nFlipEdgesOperations_;
        std::vector<Function*> repairedFunctions_; //should only be one per repairSet per inconsistent node
        std::vector<Edge*> flippedEdges_;
        std::vector<Edge*> removedEdges_;
        std::vector<Edge*> addedEdges_;

        RepairSet();
        ~RepairSet();

        int getNTopologyChanges();
        int getNRepairOperations();
        int getNAddRemoveOperations();
        int getNFlipEdgesOperations();
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
        bool topologicalError_;
        int nTopologyChanges_;
        int nRepairOperations_;
        int nAddRemoveOperations_;
        int nFlipEdgesOperations_;
        bool repaired_;
        //possible ways to repair the node;
        std::vector<RepairSet*> repairSet_;

        //debug repair type: 0 - no info; 1 - gen; 2 - part; 3 - both
        int repairType;

        InconsistentNode(std::string id, bool generalization);
        ~InconsistentNode();

        int getNTopologyChanges();
        int getNRepairOperations();
        int getNAddRemoveOperations();
        int getNFlipEdgesOperations();
        void addRepairSet(RepairSet* repairSet);
};


class InconsistencySolution {

    public:
        //map node id as key
        std::map<std::string, InconsistentNode*> iNodes_;
        //map profile as key; second map time as key; third map node as key;
        std::map<std::string, std::map<int, std::map<std::string, int > > > vlabel_;
        //map time as key; second map profile as key; second map as a vector of the id of the nodes that update in a given time, in a given profile
        //note that multiple node can update in multi-asynchronous update mode
        std::map<int, std::map<std::string, std::vector<std::string> > > updates_;
        int nTopologyChanges_;
        int nAROperations;
        int nEOperations;
        int nRepairOperations_;
        bool hasImpossibility;

        InconsistencySolution();
        ~InconsistencySolution();

        void addGeneralization(std::string id);
        void addParticularization(std::string id);
        void addTopologicalError(std::string id);
        void addVLabel(std::string profile, std::string id, int value, int time = 0);
        void addUpdate(int time, std::string profile, std::string id);
        void addRepairSet(std::string id, RepairSet* repairSet);
        int getNTopologyChanges();
        int getNRepairOperations();
        InconsistentNode* getINode(std::string id);
        void printSolution(int verboseLevel, bool printAll = true);
        void printParsableSolution(int verboseLevel);
        int compareRepairs(InconsistencySolution* solution);

};
#endif
