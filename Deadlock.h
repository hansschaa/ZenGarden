#ifndef ZEN_DEADLOCK_H
#define ZEN_DEADLOCK_H

#include <chrono>
#include <string>
#include <boost/dynamic_bitset.hpp>
#include "GameConfig.h"


using namespace std;

class Deadlock{
    public:
        static bool HasTunnel(GameConfig gameConfig);
        static bool CheckTunnel(GameConfig gameConfig, boost::dynamic_bitset<> bitset, 
        int iGap, int jGap, int iJump, int jJump, int emptyIndex);
        static void Reset();
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

#endif //ZEN_DEADLOCK_H