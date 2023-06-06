#ifndef ZEN_MYALGORITHMS_H
#define ZEN_MYALGORITHMS_H

#include <iostream>
#include <queue>
#include <unordered_set>
#include "ZenBoard.h"
using namespace std;


class MyAlgorithms{
    public:
        static void BFS(ZenBoard& zenBoard, int showPath);
        static void AStar(ZenBoard& zenBoard, int showPath);
        static void IDAStar(ZenBoard& zenBoard, int showPath);
        static void ShowMoves(ZenBoard zenBoard);
        static int Search(vector<ZenBoard>& path, int g, int bound);
};


#endif //ZEN_MYALGORITHMS_H