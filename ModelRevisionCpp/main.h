#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>
#include "Network.h"


void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file);

void modelRevision(std::string input_file_network);

std::vector<Solution*> checkConsistencyFunc(std::string input_file_network, int & optimization);

std::vector<Function*> repairFuncConsistency(Solution* repairSet, int & optimization);

bool isFuncConsistentWithLabel(Solution* labeling, Function* f);

bool checkPointFunction(Solution* labeling, Function* f, bool generalize);

Function* repairFuncConsistencyFlippingEdge(Solution* labeling, Function* f, bool generalize);


#endif

