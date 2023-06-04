#ifndef ZEN_STATISTICS_H
#define ZEN_STATISTICS_H

#include <chrono>

class Statistics{
    public:
        static int totalNodesExpanded;
        static int turn;
        static int solutionLength;
        static std::chrono::high_resolution_clock::time_point start;
        static std::chrono::high_resolution_clock::time_point end;

        static void Print();
};






#endif //ZEN_STATISTICS_H