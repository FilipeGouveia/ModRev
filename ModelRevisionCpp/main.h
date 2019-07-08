#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>
#include "Network.h"


void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file);

void modelRevision(std::string input_file_network);

std::vector<InconsistencySolution*> checkConsistencyFunc(std::string input_file_network, int & optimization);

void repairInconsistencies(InconsistencySolution* inconsistency);

void repairNodeConsistency(InconsistencySolution* inconsistency, InconsistentNode* iNode);

bool isFuncConsistentWithLabel(InconsistencySolution* labeling, Function* f);

bool isFuncConsistentWithLabel(InconsistencySolution* labeling, Function* f, std::string profile);

bool checkPointFunction(InconsistencySolution* labeling, Function* f, bool generalize);

bool checkPointFunction(InconsistencySolution* labeling, Function* f, std::string profile, bool generalize);

void repairNodeConsistencyWithTopologyChanges(InconsistencySolution* inconsistency, InconsistentNode* iNode);

bool repairNodeConsistencyFlippingEdges(InconsistencySolution* solution, InconsistentNode* iNode, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges);

void repairNodeConsistencyByRegulators(InconsistencySolution* solution, InconsistentNode* iNode);

std::vector<std::vector<Edge *>> getEdgesCombinations(std::vector<Edge *> edges, int n);

std::vector<std::vector<Edge *>> getEdgesCombinations(std::vector<Edge *> edges, int n, int indexStart);

//fg
bool searchComparableFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges);
bool searchNonComparableFunctions(InconsistencySolution* inconsistency, InconsistentNode* iNode, std::vector<Edge*> flippedEdges, std::vector<Edge*> addedEdges, std::vector<Edge*> removedEdges);
bool isIn(std::vector<Function*> list, Function* item);
void printAllFunctions(int dimension);
bool myFunctionCompare(Function * f1, Function * f2);
bool isFunctionInBottomHalf(Function *f);
bool isFunctionInBottomHalfByState(Function *f);

int nFuncInconsistWithLabel(InconsistencySolution* labeling, Function* f);
int nFuncInconsistWithLabel(InconsistencySolution* labeling, Function* f, std::string profile);

bool getFunctionValue(Function * f, std::map<std::string,int> input);

#endif
