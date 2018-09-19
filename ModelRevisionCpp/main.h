#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>
#include "Network.h"


void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file);

void modelRevision(std::string input_file_network);

std::vector<FunctionRepairs*> checkConsistencyFunc(std::string input_file_network, int & optimization);

std::vector<Function*> repairFuncConsistency(std::string input_file_network, FunctionRepairs* repairSet, int & optimization);

#endif

