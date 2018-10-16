#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>
#include "Network.h"


void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file);

void modelRevision(std::string input_file_network);

std::vector<FunctionInconsistencies*> checkConsistencyFunc(std::string input_file_network, int & optimization);

std::vector<Function*> repairFuncConsistency(FunctionInconsistencies* repairSet, int & optimization);

bool isFuncConsistentWithLabel(FunctionInconsistencies* labeling, Function* f);

bool checkPointFunction(FunctionInconsistencies* labeling, Function* f, bool generalize);

Function* repairFuncConsistencyFlippingEdge(FunctionInconsistencies* labeling, Function* f, bool generalize);


#endif

