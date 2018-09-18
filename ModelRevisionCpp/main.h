#ifndef MAIN_H_
#define MAIN_H_

#include <string>


void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file);

void modelRevision(std::string input_file_network);
#endif
