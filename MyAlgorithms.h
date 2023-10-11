#ifndef ZEN_MYALGORITHMS_H
#define ZEN_MYALGORITHMS_H

#include <iostream>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <boost/functional/hash.hpp>
#include <boost/dynamic_bitset.hpp>
#include "ZenBoard.h"
#include "Utils.h"
#include "Deadend.h"
#include "MyIndividual.h"

using namespace std;

class MyAlgorithms{
    public:
        static void BFS(ZenBoard& zenBoard);
        static int AStar(MyIndividual& myIndividual);
        static bool AStar(ZenBoard& zenBoard);
        static void IDAStar(ZenBoard& zenBoard);
        static int InnerSearch(stack<ZenBoard> path, int g, int bound);  
        static int TTInnerSearch(stack<ZenBoard> path, int g, int bound);  
        static void ShowMoves(ZenBoard zenBoard, unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals>& map);
        static int GetLenght(ZenBoard zenBoard, unordered_map<ZenBoard, ZenBoard, Utils::GetHashCode, Utils::Equals>& map);
        
};


#endif //ZEN_MYALGORITHMS_H