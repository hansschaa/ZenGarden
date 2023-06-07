#ifndef ZEN_ZENBOARD_H
#define ZEN_ZENBOARD_H

#include <bitset>
#include "Vector2.h"


using namespace std;

class ZenBoard {
public:
    bitset<36> garden
            {string("000000001100000000000000000100000000")};
    bitset<36> player
            {string("000000000000000000000000000000000000")};

    bitset<36> GetWholeBoard();

    int h = 0,g = 0;
    Vector2<int> lastDir = Vector2<int>(0,0);
    
    int GetF(){
        return h+g;
    }

    //Count spaces whit 1s
    void CompH();
    void Print();

    bool operator==(const ZenBoard& other) const {
        auto equals = (garden == other.garden) && (player == other.player); 
        return equals;
    }
};

#endif //ZEN_ZENBOARD_H