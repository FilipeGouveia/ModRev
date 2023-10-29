#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>
#include "Network.h"


int process_arguments(const int argc, char const * const * argv);

void printHelp();

void modelRevision();

std::vector<InconsistencySolution*> checkConsistency(int & optimization);
void printConsistency(std::vector<InconsistencySolution*> inconsistencies, int optimization);

void repairInconsistencies(InconsistencySolution* inconsistency);

void repairNodeConsistency(InconsistencySolution* inconsistency, InconsistentNode* iNode);

std::vector<std::vector<Edge *>> getEdgesCombinations(std::vector<Edge *> edges, int n);
std::vector<std::vector<Edge *>> getEdgesCombinations(std::vector<Edge *> edges, int n, int indexStart);

bool repairNodeConsistencyFlippingEdges(InconsistencySolution* solution, InconsistentNode* iNode, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges);

bool repairNodeConsistencyFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges);

int nFuncInconsistentWithLabel(InconsistencySolution* labeling, Function* f);
int nFuncInconsistentWithLabel(InconsistencySolution* labeling, Function* f, std::string profile);

bool searchComparableFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges, bool generalize);
bool searchNonComparableFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges);

bool isFuncConsistentWithLabel(InconsistencySolution* labeling, Function* f);
bool isFuncConsistentWithLabel(InconsistencySolution* labeling, Function* f, std::string profile);

bool isFunctionInBottomHalf(Function *f);
bool isFunctionInBottomHalfByState(Function *f);
bool getFunctionValue(Function * f, std::map<std::string,int> input);

bool isIn(std::vector<Function*> list, Function* item);



/*
bool checkPointFunction(InconsistencySolution* labeling, Function* f, bool generalize);

bool checkPointFunction(InconsistencySolution* labeling, Function* f, std::string profile, bool generalize);

void repairNodeConsistencyWithTopologyChanges(InconsistencySolution* inconsistency, InconsistentNode* iNode);

void repairNodeConsistencyByRegulators(InconsistencySolution* solution, InconsistentNode* iNode);


void printAllFunctions(int dimension);
bool myFunctionCompare(Function * f1, Function * f2);
*/

#endif
