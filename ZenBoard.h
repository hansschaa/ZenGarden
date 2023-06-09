#ifndef ZEN_ZENBOARD_H
#define ZEN_ZENBOARD_H

#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include "Vector2.h"


using namespace std;

class ZenBoard {
public:
    string a = "100000000000000000000000000000000000";
    boost::dynamic_bitset<> garden = boost::dynamic_bitset<> (a);
    boost::dynamic_bitset<> player = boost::dynamic_bitset<> (a);

    

    int h = 0,g = 0;
    Vector2<int> lastDir = Vector2<int>(0,0);
    
   

    //Count spaces whit 1s
    void CompH();
    void Print();
    boost::dynamic_bitset<> GetWholeBoard();

    int GetF(){
        return h+g;
    }

    bool operator==(const ZenBoard& other) const {
        auto equals = (garden == other.garden) && (player == other.player); 
        return equals;
    }
};

#endif //ZEN_ZENBOARD_H