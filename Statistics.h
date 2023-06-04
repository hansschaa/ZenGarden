#ifndef ZEN_STATISTICS_H
#define ZEN_STATISTICS_H

#include <chrono>
#include <string>

using namespace std;

class Statistics{
    public:
        static int totalNodesExpanded;
        static int turn;
        static int solutionLength;
        static std::chrono::high_resolution_clock::time_point start;
        static std::chrono::high_resolution_clock::time_point end;

        static void Print(string algorithmName);
};






#endif //ZEN_STATISTICS_H