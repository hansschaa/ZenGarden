#ifndef ZEN_ZENBOARD_H
#define ZEN_ZENBOARD_H

#include <bitset>
#include <boost/dynamic_bitset.hpp>
#include "Vector2.h"


using namespace std;

class ZenBoard {
public:
    
    std::bitset<36> garden;
    std::bitset<36> player;

    int h = 0,g = 0;
    Vector2<int> lastDir = Vector2<int>(0,0);
    
    void CompH();
    void Print();
    void PrintBitset();
    bitset<36> GetWholeBoard();
    void Reset();

    int GetF(){
        return h+g;
    }

    bool operator==(const ZenBoard& other) const {
        return garden == other.garden && player == other.player;
    }

    size_t GetHashCode();
};

#endif //ZEN_ZENBOARD_H