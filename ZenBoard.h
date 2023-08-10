#ifndef ZEN_ZENBOARD_H
#define ZEN_ZENBOARD_H

#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include "Vector2.h"


using namespace std;

class ZenBoard {
public:
    
    boost::dynamic_bitset<> garden = boost::dynamic_bitset<> (string("00"));
    boost::dynamic_bitset<> player = boost::dynamic_bitset<> (string("00"));

    int h = 0,g = 0;
    Vector2<int> lastDir = Vector2<int>(0,0);
    
    void CompH();
    void Print();
    void PrintBitset();
    boost::dynamic_bitset<> GetWholeBoard();

    int GetF(){
        return h+g;
    }

    bool operator==(const ZenBoard& other) const {
        return garden == other.garden && player == other.player;
    }

    size_t GetHashCode();
};

#endif //ZEN_ZENBOARD_H