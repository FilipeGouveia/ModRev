#ifndef MAIN_H_
#define MAIN_H_

#include <string>
#include <vector>
#include "Network.h"
#include <iostream>
#include <fstream>


void process_arguments(const int argc, char const * const * argv, std::string & input_file_network, std::string & output_file, std::string & log_file, int & f_prob, int & e_prob, int & r_prob, int & a_prob);
bool getProbability(int prob);
void modelMessing(std::ofstream & log_s, int f_prob, int e_prob, int r_prob, int a_prob);
void printNetwork(std::string output_file);


#endif
