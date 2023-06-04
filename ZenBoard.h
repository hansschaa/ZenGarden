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

    int h,g;
    
    int GetF(){
        return h+g;
    }

    int CompH(){
        return garden.size()-garden.count();
    }

};

#endif //ZEN_ZENBOARD_H