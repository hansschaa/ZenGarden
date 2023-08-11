#ifndef ZEN_DEADEND_H
#define ZEN_DEADEND_H

#include <chrono>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include <bitset>
#include "GameConfig.h"
#include "TunnelInfo.h"
#include "ZenBoard.h"


using namespace std;

class Deadend{
    public:
        //static const bitset<16> firstCol, lastCol, firstRow, lastRow;
        static const bitset<36> firstCol, lastCol, firstRow, lastRow;
        static bitset<36> r_Corner, r_FreeCorner, u_Corner, u_FreeCorner, d_Corner, d_FreeCorner,l_Corner, l_FreeCorner,copyBitset;

        //static const bitset<64> firstCol, lastCol, firstRow, lastRow;
        //static bitset<64> r_Corner, r_FreeCorner, u_Corner, u_FreeCorner, d_Corner, d_FreeCorner,l_Corner, l_FreeCorner,copyBitset;
        
        
        static bool HasDeadend(ZenBoard zenBoard);
        static bool CheckAlleys(ZenBoard zenBoard);
        static bool CheckCorners(ZenBoard zenBoard);
};

#endif //ZEN_DEADEND_H