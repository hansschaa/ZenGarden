#ifndef ZEN_DEADEND_H
#define ZEN_DEADEND_H

#include <chrono>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include "GameConfig.h"
#include "ZenBoard.h"


using namespace std;

class Deadend{
    public:
        static bool HasDeadend(GameConfig& gameConfig, ZenBoard& zenBoard);
        static bool CheckDeadend(GameConfig& gameConfig, boost::dynamic_bitset<> bitset, 
        int iGap, int jGap, int iJump, int jJump, int emptyIndex, 
        ZenBoard& zenBoard);
        /*static int totalNodesExpanded;
        static int turn;
        static int solutionLength;
        static int isTimeOut;
        static int maxMilliseconds;
        static std::chrono::high_resolution_clock::time_point start;
        static std::chrono::high_resolution_clock::time_point end;

        //H Time
        static std::chrono::high_resolution_clock::time_point h_start;
        static std::chrono::high_resolution_clock::time_point h_end;
        static double h_total;

        static std::chrono::high_resolution_clock::time_point gen_start;
        static std::chrono::high_resolution_clock::time_point gen_end;
        static double gen_total;

        static void Print(string algorithmName);
        static bool IsTimeOut();*/
};

#endif //ZEN_DEADEND_H