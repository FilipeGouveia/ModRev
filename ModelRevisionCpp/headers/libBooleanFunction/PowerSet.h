#ifndef POWERSET_H_
#define POWERSET_H_

#include <string>
#include <map>
#include <set>
#include <vector>
#include <boost/dynamic_bitset.hpp>
//#include <bitset>

namespace BooleanFunction
{

    class PowerSet {

        public:

            std::set< boost::dynamic_bitset<> > powerset_base_;
            int dimension_;

            PowerSet(int d);
            ~PowerSet();

            int getDimension();

            std::set< boost::dynamic_bitset<> > getIndependent(boost::dynamic_bitset<> term);
            std::set< boost::dynamic_bitset<> > getIndependent(std::set< boost::dynamic_bitset<> > terms);
            std::set< boost::dynamic_bitset<> > getMaxIndependent(std::set< boost::dynamic_bitset<> > terms);
            bool isContained(boost::dynamic_bitset<> term, std::set< boost::dynamic_bitset<> > setToTest, bool strictly = false);
            std::set< boost::dynamic_bitset<> > filterContained(std::set< boost::dynamic_bitset<> > setTofilter, std::set< boost::dynamic_bitset<> > setToTest, bool strictly = false);

            bool isDominated(boost::dynamic_bitset<> term, std::set< boost::dynamic_bitset<> > setToTest, bool strictly = false);
            //remove all the dominated terms
            std::set< boost::dynamic_bitset<> > filterDominated(std::set< boost::dynamic_bitset<> > setTofilter, std::set< boost::dynamic_bitset<> > setToTest, bool strictly = false);

            //remove all the non dominated terms
            std::set< boost::dynamic_bitset<> > filterNonDominated(std::set< boost::dynamic_bitset<> > setTofilter, std::set< boost::dynamic_bitset<> > setDominators, bool strictly = false);


            std::set< boost::dynamic_bitset<> > getParents(boost::dynamic_bitset<> term);
            std::set< boost::dynamic_bitset<> > getChildren(boost::dynamic_bitset<> term);

            std::set< boost::dynamic_bitset<> > getAllParents(std::set< boost::dynamic_bitset<> > terms);
            std::set< boost::dynamic_bitset<> > getAllChildren(std::set< boost::dynamic_bitset<> > terms);

            bool isDegenerated(std::set< boost::dynamic_bitset<> > terms);


    };
}

#endif