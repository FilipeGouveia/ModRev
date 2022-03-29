#ifndef BOOLEANFUNCTION_H_
#define BOOLEANFUNCTION_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <boost/dynamic_bitset.hpp>

namespace BooleanFunction
{

    #define VERSION_MAJOR 0
    #define VERSION_MINOR 0
    #define VERSION_PATCH 0

    static std::string GetVersion() {return std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_PATCH);}
    static int GetVersionMajor() {return VERSION_MAJOR;}
    static int GetVersionMinor() {return VERSION_MINOR;}
    static int GetVersionPatch() {return VERSION_PATCH;}


    // Boolean function represented in DNF, i.e,
    // a disjunction of terms where each term is a conjunction of boolean variables
    // where each variable is represented by a string
    class Function
    {
    public:
        Function();
        Function(std::string outputVariable);

        ~Function();

        // sets the target output variable name
        void setOutputVariable(std::string target);

        // returns the target output variable name
        std::string getOutputVariable();

        // method to add a variable to a term
        void addVariableToTerm(int termId, std::string variable);

        // returns a map with term identifier and the correspondent vector of variables
        std::map<int, std::set<std::string>> getTerms();

        // returns the number of terms
        int getNTerms();

        // returns the dimension of the Boolean function, i.e.
        // returns the number of variables it contains
        int getDimension();

        // returns a map between variable name and integer variable identifier
        std::map<std::string,int> getVariableMap();

        // returns a map between variable integer identifier and its name
        std::map<int, std::string> getVariableMapById();

        // returns true if a given function f is equal to this function
        bool isEqual(Function* f);

        // returns the level of the Boolean function
        std::vector<int> getLevel();

        // compares the level of a given function with this function
        // returns
        //      -1 -> this function has a lower level
        //      0  -> this function has the same level
        //      1  -> this function has a greater level
        int compareLevel(Function * f);
        int compareLevel(std::vector<int> fullLevel);

        // returns a vector of parents of the function
        std::vector<Function*> getParents();

        // returns a vector of children of the function
        std::vector<Function*> getChildren();

        class Impl;
    
    private:

        std::string outputVariable_;
        std::map<int, std::set<std::string> > terms_;
        std::map<std::string,int> variableMap_;
        std::map<int, std::string> variableMapById_;
        int nTerms_;
        std::vector<int> level_;

        std::set< boost::dynamic_bitset<> > convertToBitset();

        Function * convertToFunction(std::set< boost::dynamic_bitset<> > bitset);

 
    };
    
    // returns a string representation of a function
    std::string PrintFunction(Function *function);

    // returns a string representation of a function level
    std::string PrintFunctionLevel(Function *function);

    Function * ParseFunction(std::string functionText);


}

#endif