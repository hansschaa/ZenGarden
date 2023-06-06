#ifndef ZEN_MYALGORITHMS_H
#define ZEN_MYALGORITHMS_H

#include <iostream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include "ZenBoard.h"
#include "Utils.h"
using namespace std;


class MyAlgorithms{
    public:
        static void BFS(ZenBoard& zenBoard);
        static void AStar(ZenBoard& zenBoard);
        static void IDAStar(ZenBoard& zenBoard);
        static void ShowMoves(ZenBoard zenBoard, unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals>& map);
        static int Search(vector<ZenBoard>& path, int g, int bound);
};


#endif //ZEN_MYALGORITHMS_H